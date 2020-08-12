#include "glbl.h"
#include "fnv.h"
#include "bool.h"
#include "table.h"

// IMPORTANT: Read this for query flags
#include "linked_shared.h"

/* Internal magic */

/* Currently using "hardered" FNV-1A hashes for the keys.
 * It may be worth swapping this out for xxHash or SIPHASH24 or something.
 * (I think Python maybe uses SIPHASH???)
 */
#define RadixTable_HashKey(a) RadixAbstract_fnv2r_64(a)

/* Table initialization */

RadixTable RadixTable_New()
    // This allows for a single shared point for initialization! :)
    { RadixTable table = {0, NULL, false}; return table; }

/* Key iteration */

bool RadixTable_KeyIteratorCheckElement(
    RadixTableKeyIterator *ki,
    RadixTableQuery *kq)
{
    // This weird if structure allows you to query for multiple things
    // simultaneously instead of making a single query.
    // It's ugly, but it works. I swear.
    if (kq->query_for & QUERY_INDEX)
        if (ki->index == kq->index && ki->element) return true;
    if (kq->query_for & QUERY_KEY)
        if (ki->element->keyHash == kq->keyHash) return true;
    if (kq->query_for & QUERY_KEY_CONTAINS)
        if (RadixAbstract_BlobContains(
            ki->element->key, *kq->key, kq->containsInterval))
            return true;
    if (kq->query_for & QUERY_VALUE)
        if (RadixAbstract_BlobEquals(ki->element->value, *kq->value))
            return true;
    if (kq->query_for & QUERY_VALUE_CONTAINS)
        if (RadixAbstract_BlobContains
            (ki->element->value, *kq->value, kq->containsInterval))
            return true;

    return false;
}

/* Presence checking/Key to struct conversion */

RadixTableQuery RadixTable_ConstructQuery(
    char query_for,
    RadixMemoryBlob *key,
    RadixMemoryBlob *value,
    unsigned long long index,
    unsigned int containsInterval)
{
    // If we're even looking for the key, create hash for optimized querying.
    uint64_t hkey = 0;
    if (query_for & QUERY_KEY)
        hkey = RadixTable_HashKey(*key);

    RadixTableQuery query = {
        query_for, hkey, key, value, index, containsInterval
    };

    return query;
}

RadixTableQueryResult RadixTable_Query(
    RadixTable *table,
    RadixTableQuery query)
{
    // Begin the key iterator.
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableQueryResult result;
    result.found = false;

    // Check if the current element is not d e a d each iteration.
    while ((result.current = RadixIterator_Get(&keys)))
    {   
        if (RadixTable_KeyIteratorCheckElement(&keys, &query))
        {
            /* If found, store informaton about the current, next and previous
             * elements.
             */
            result.index.present = BOOLIFY(result.current);
            result.index.index = RadixIterator_GetIndex(&keys);
            result.next = RadixIterator_GetNext(&keys);
            result.previous = RadixIterator_GetPrev(&keys);
            result.found = true;

            break;
        }

        RadixTable_KeyIteratorNext(&keys);
    }

    if (!result.found)
    {
        result.current = NULL;
        result.index.present = false;
    }

    return result;
}

RadixTableElement * RadixTable_Find(RadixTable *table, RadixMemoryBlob key)
{
    RadixTableQuery query = RadixTable_ConstructQuery(
        QUERY_KEY, &key, NULL, 0, 1);
    return RadixTable_Query(table, query).current;
}

bool RadixTable_In(RadixTable *table, RadixMemoryBlob key)
    // Converts RadixTable_Find to boolean form
    { return BOOLIFY(RadixTable_Find(table, key)); }

/* Value retreival retrieval */

RadixMemoryBlob * RadixTable_GetItem(RadixTable *table, RadixMemoryBlob key)
{
    // Find an element by key and return the value - what most people will need
    RadixTableElement *element = RadixTable_Find(table, key);

    // Return null pointer if not found
    if (!element)
        return NULL;

    return &(element->value);
}

/* Table Element Configuration/Manipulation */

void RadixTable_SetItem(
    RadixTable *table,
    RadixMemoryBlob key,
    RadixMemoryBlob value)
{
    RadixTableElement *element = RadixTable_Find(table, key);

    // If the element doesn't exist, we have to do some funky things
    if (!element)
    {
        RadixTableElement *passing_element = (table->first_element);

        if(table->insert_at_end)
        {
            if(passing_element)
            {
                while (passing_element->next_element)
                    passing_element = element->next_element;

                passing_element->next_element = malloc(
                    sizeof(RadixTableElement));
                element = passing_element->next_element;
            } else {
                table->first_element = malloc(sizeof(RadixTableElement));
                element = table->first_element;
            }
            element->next_element = NULL;
        } else {
            table->first_element = malloc(sizeof(RadixTableElement));
            element = table->first_element;
            element->next_element = passing_element;
        }

        // Create the new key hash
        element->keyHash = RadixTable_HashKey(key);
        element->key = RadixAbstract_MallocCopy(key);

        table->length++;
    } else {
        RadixAbstract_DestroyBlob(&(element->value));
    }

    // Malloc a new blob for the value to isolate the table.
    element->value = RadixAbstract_MallocCopy(value);
}

bool RadixTable_ChangeKey(
    RadixTable *table,
    RadixMemoryBlob key,
    RadixMemoryBlob new_key)
{
    RadixTableElement *element = RadixTable_Find(table, key);

    if (!element) return false;
    
    RadixAbstract_DestroyBlob(&(element->key));
    element->keyHash = RadixTable_HashKey(new_key);
    element->key = RadixAbstract_MallocCopy(new_key);

    return true;
}

/* Memory usage reduction/Item destruction/Table destruction */

bool RadixTable_DestroyItem(RadixTable *table, RadixMemoryBlob key)
{
    RadixTableQuery query_in = RadixTable_ConstructQuery(
        QUERY_KEY, &key, NULL, 0, 1);
    RadixTableQueryResult query = RadixTable_Query(table, query_in);

    if (!query.found)
        return false;

    if (query.previous)
        query.previous->next_element = query.next;
    else table->first_element = query.next;

    RadixAbstract_DestroyBlob(&(query.current->key));
    RadixAbstract_DestroyBlob(&(query.current->value));
    free(query.current);

    table->length--;

    return true;
}

void RadixTable_DestroyTable(RadixTable *table)
{
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableElement *element;

    while ((element = RadixIterator_Get(&keys)))
    {
        RadixAbstract_DestroyBlob(&(element->key));
        RadixAbstract_DestroyBlob(&(element->value));
        free(element);

        RadixTable_KeyIteratorNext(&keys);
    }

    table->first_element = NULL;
    table->length = 0;
}

/* Table searching */

RadixMemoryBlob * RadixTable_ValueGet(RadixTable *table, RadixMemoryBlob value)
{
    return &(RadixTable_Query(table,
        RadixTable_ConstructQuery(QUERY_VALUE, NULL, &value, 0, 1)
    ).current->key);
}

/* Index manipulation */

RadixMemoryBlob * RadixTable_KeyByIndex(
    RadixTable *table,
    unsigned long long index)
{
    return &(RadixTable_Query(table,
        RadixTable_ConstructQuery(QUERY_VALUE, NULL, NULL, index, 1)
    ).current->key);
}

RadixIndex RadixTable_IndexByKey(
    RadixTable *table,
    RadixMemoryBlob key)
{
    return RadixTable_Query(
        table, RadixTable_ConstructQuery(QUERY_KEY, &key, NULL, 0, 1)
    ).index;
}

/* Pythonic updates */

void RadixTable_Update(RadixTable *dest, RadixTable *src)
{
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(src);
    RadixTableElement *element;

    while ((element = RadixIterator_Get(&keys)))
    {
        RadixTable_SetItem(dest, element->key, element->value);

        RadixTable_KeyIteratorNext(&keys);
    }
}
