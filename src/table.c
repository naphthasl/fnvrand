#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "fnv.h"
#include "bool.h"
#include "table.h"

/* Internal magic */

uint64_t RadixTable_HashKey(RadixMemoryBlob key)
    /* Currently using "hardered" FNV-1A hashes for the keys.
     * It may be worth swapping this out for xxHash or SIPHASH24 or something.
     * (I think Python maybe uses SIPHASH???)
     */
    { return RadixAbstract_fnv2r_64(key); }

/* Table initialization */

RadixTable RadixTable_New()
    // This allows for a single shared point for initialization! :)
    { RadixTable table = {0, NULL, false}; return table; }

/* Property checking */

unsigned long long RadixTable_Length(RadixTable *table)
    // Ensures the user doesn't need to interact with the struct directly
    { return table->length; }

/* Key iteration */

RadixTableKeyIterator RadixTable_NewKeyIterator(RadixTable *table)
{
    /* Create a key iterator - important, since it means you don't need to
     * create an array containing every key hash or something.
     */

    RadixTableKeyIterator ki = {
        table, table->first_element, 0, NULL, NULL, false
    };
    
    if (table->first_element) ki.next = ki.element->next_element;

    return ki;
}

RadixTableElement * RadixTable_KeyIteratorGet(RadixTableKeyIterator *ki)
    { return ki->element; }

RadixTableElement * RadixTable_KeyIteratorGetPrev(RadixTableKeyIterator *ki)
    { return ki->previous; }

RadixTableElement * RadixTable_KeyIteratorGetNext(RadixTableKeyIterator *ki)
    { return ki->next; }

unsigned long long RadixTable_KeyIteratorIndex(RadixTableKeyIterator *ki)
    { return ki->index; }

void RadixTable_KeyIteratorNext(RadixTableKeyIterator *ki)
{
    // If the current element is not a null pointer, move up the linked list.
    if (ki->element) 
    {
        ki->previous = ki->element;
        ki->element = ki->element->next_element;
        if (ki->element) ki->next = ki->element->next_element;
        else ki->next = NULL;
        
        ki->index++;
    } else {
        ki->exhausted = true;
    }
}

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
    while ((result.current = RadixTable_KeyIteratorGet(&keys)))
    {   
        if (RadixTable_KeyIteratorCheckElement(&keys, &query))
        {
            /* If found, store informaton about the current, next and previous
             * elements.
             */
            result.index.present = BOOLIFY(result.current);
            result.index.index = RadixTable_KeyIteratorIndex(&keys);
            result.next = RadixTable_KeyIteratorGetNext(&keys);
            result.previous = RadixTable_KeyIteratorGetPrev(&keys);
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
        // Destroy the original value to prevent a memory leak
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

    while ((element = RadixTable_KeyIteratorGet(&keys)))
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

RadixTableIndex RadixTable_IndexByKey(
    RadixTable *table,
    RadixMemoryBlob key)
{
    return RadixTable_Query(
        table, RadixTable_ConstructQuery(QUERY_KEY, &key, NULL, 0, 1)
    ).index;
}

bool RadixTable_IndexStructExists(RadixTableIndex index)
    { return index.present; }

unsigned long long RadixTable_IndexStructPosition(RadixTableIndex index)
    { return index.index; }

/* Pythonic updates */

void RadixTable_Update(RadixTable *dest, RadixTable *src)
{
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(src);
    RadixTableElement *element;

    while ((element = RadixTable_KeyIteratorGet(&keys)))
    {
        RadixTable_SetItem(dest, element->key, element->value);

        RadixTable_KeyIteratorNext(&keys);
    }
}
