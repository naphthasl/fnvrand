#include <stdint.h>
#include "fnv.h"
#include "bool.h"

/* Persistent structures */

typedef struct RadixTableElement {
    // This allows for better iterative performance when searching for keys
    uint64_t keyHash;
    // This provides a key/value pair similar to Python's dicts
    RadixMemoryBlob key;
    RadixMemoryBlob value;
    // Since this is based on the concept of a linked list...
    RadixTableElement *next_element;
} RadixTableElement;

typedef struct RadixTable {
    // The length of the table, which changes during SetItem and DestroyItem
    unsigned long long length;
    // The very first element of the table, which starts the chain.
    RadixTableElement *first_element;
} RadixTable;

typedef struct RadixTableIndex {
    bool present;
    unsigned long long index;
} RadixTableIndex;

typedef struct RadixTableKeyIterator {
    // Points back to the table, so you always know the source.
    RadixTable *table;
    // The current element that the iterator is hovering over.
    RadixTableElement *element;
    // The current index the iterator is hovering over, starting from 0.
    unsigned long long index;
} RadixTableKeyIterator;

typedef struct RadixTableQueryResult {
    bool found;
    RadixTableElement *previous;
    RadixTableIndex previous_index;
    RadixTableElement *current;
    RadixTableIndex current_index;
    RadixTableElement *next;
    RadixTableIndex next_index;
} RadixTableQueryResult;

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
    { RadixTable table = {0, 0}; return table; }

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

    RadixTableKeyIterator ki = {table, table->first_element, 0};
    
    return ki;
}

RadixTableElement * RadixTable_KeyIteratorGet(RadixTableKeyIterator *ki)
    { return ki->element; }

unsigned long long RadixTable_KeyIteratorIndex(RadixTableKeyIterator *ki)
    { return ki->index; }

void RadixTable_KeyIteratorNext(RadixTableKeyIterator *ki)
{
    // If the current element is not a null pointer, move up the linked list.
    if (ki->element) { ki->element = ki->element->next_element; ki->index++; }
}

/* Presence checking/Key to struct conversion */

RadixTableQueryResult RadixTable_Query(
    RadixTable *table,
    char query_for,
    RadixMemoryBlob *key,
    RadixMemoryBlob *value,
    unsigned long long index)
{
    // If we're even looking for the key, create hash for optimized querying.
    uint64_t hkey = 0;
    if (query_for & QUERY_KEY)
        hkey = RadixTable_HashKey(*key);
    
    // Begin the key iterator.
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableQueryResult result;

    while ((result.current = RadixTable_KeyIteratorGet(&keys)))
    {   
        // Gather index info for the current element.
        result.current_index.present = BOOLIFY(result.current);
        result.current_index.index = RadixTable_KeyIteratorIndex(&keys);

        // This weird if structure allows you to query for multiple things
        // simultaneously instead of making a single query.
        // It's ugly, but it works. I swear.
        if (query_for & QUERY_INDEX)
            if (result.current_index.index == index &&
                result.current_index.present) result.found = true;
        if (query_for & QUERY_KEY)
            if (result.current->keyHash == hkey) result.found = true;
        if (query_for & QUERY_KEY_CONTAINS)
            if (RadixAbstract_BlobContains(result.current->key, *key, 1))
                result.found = true;
        if (query_for & QUERY_VALUE)
            if (RadixAbstract_BlobEquals(result.current->value, *value))
                result.found = true;
        if (query_for & QUERY_VALUE_CONTAINS)
            if (RadixAbstract_BlobContains(result.current->value, *value, 1))
                result.found = true;

        if (result.found)
        {
            // If found, get the very next element, store it, then break.
            RadixTable_KeyIteratorNext(&keys);
            result.next = RadixTable_KeyIteratorGet(&keys);
            result.next_index.present = BOOLIFY(result.next);
            result.next_index.index = RadixTable_KeyIteratorIndex(&keys);

            break;
        }

        // If not found, remember the previous element and move on to the next.
        result.previous = RadixTable_KeyIteratorGet(&keys);
        result.previous_index.present = BOOLIFY(result.previous);
        result.previous_index.index = RadixTable_KeyIteratorIndex(&keys);
        RadixTable_KeyIteratorNext(&keys);
    }

    if (!result.found)
    {
        result.current = NULL;
        result.current_index.present = false;
    }

    return result;
}

RadixTableElement * RadixTable_Find(RadixTable *table, RadixMemoryBlob key)
    { return RadixTable_Query(table, QUERY_KEY, &key, NULL, 0).current; }

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

        if(passing_element)
        {
            while (passing_element->next_element)
                passing_element = element->next_element;

            passing_element->next_element = malloc(sizeof(RadixTableElement));
            element = passing_element->next_element;
        } else {
            table->first_element = malloc(sizeof(RadixTableElement));
            element = table->first_element;
        }

        element->next_element = NULL;
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
    RadixTableQueryResult query = RadixTable_Query(
        table, QUERY_KEY, &key, NULL, 0);

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
    return &(RadixTable_Query(
        table, QUERY_VALUE, NULL, &value, 0).current->key);
}

/* Index manipulation */

RadixMemoryBlob * RadixTable_KeyByIndex(
    RadixTable *table,
    unsigned long long index)
{
    return &(RadixTable_Query(
        table, QUERY_INDEX, NULL, NULL, index).current->key);
}

RadixTableIndex RadixTable_IndexByKey(
    RadixTable *table,
    RadixMemoryBlob key)
{
    return RadixTable_Query(table, QUERY_KEY, &key, NULL, 0).current_index;
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