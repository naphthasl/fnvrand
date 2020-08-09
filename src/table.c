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

typedef struct RadixTableKeyIterator {
    // Points back to the table, so you always know the source.
    RadixTable *table;
    // The current element that the iterator is hovering over.
    RadixTableElement *element;
    // The current index the iterator is hovering over, starting from 1.
    unsigned long long index;
} RadixTableKeyIterator;

/* Internal magic */

uint64_t RadixTable_HashKey(RadixMemoryBlob key)
    /* Currently using "hardered" FNV-1A hashes for the keys.
     * It may be worth swapping this out for xxHash or SIPHASH24 or something.
     * (I think Python maybe uses SIPHASH???)
     */
    { return RadixAbstract_fnv2r_64(&key); }

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
    /* Create a key iterator - important, since it means you don't need to
     * create an array containing every key hash or something.
     */
    { RadixTableKeyIterator ki = {table, table->first_element, 1}; return ki; }

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

RadixTableElement * RadixTable_Find(RadixTable *table, RadixMemoryBlob key)
{
    uint64_t hkey;

    // Create a new key iterator so that we can search the table
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableElement *element;

    // Only calculate the hash if there are actually elements in the table
    if (RadixTable_KeyIteratorGet(&keys)) hkey = RadixTable_HashKey(key);

    while ((element = RadixTable_KeyIteratorGet(&keys)))
    {
        // Compare each key hash and return when found
        if (element->keyHash == hkey) return element;
        RadixTable_KeyIteratorNext(&keys);
    }

    // Return null pointer if there are no matching keys
    return NULL;
}

bool RadixTable_In(RadixTable *table, RadixMemoryBlob key)
    // Converts RadixTable_Find to boolean form
    { return (RadixTable_Find(table, key)) ? true : false; }

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
        element->key = RadixAbstract_MallocCopy(&key);

        table->length++;
    } else {
        // Destroy the original value to prevent a memory leak
        RadixAbstract_DestroyBlob(&(element->value));
    }

    // Malloc a new blob for the value to isolate the table.
    element->value = RadixAbstract_MallocCopy(&value);
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
    element->key = RadixAbstract_MallocCopy(&new_key);

    return true;
}

/* Memory usage reduction/Item destruction/Table destruction */

bool RadixTable_DestroyItem(RadixTable *table, RadixMemoryBlob key)
{
    uint64_t hkey = RadixTable_HashKey(key);
    
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableElement *previous_element = NULL;
    RadixTableElement *element;
    RadixTableElement *next_element;

    while ((element = RadixTable_KeyIteratorGet(&keys)))
    {
        RadixTable_KeyIteratorNext(&keys);

        if (element->keyHash == hkey)
        {
            next_element = RadixTable_KeyIteratorGet(&keys);

            RadixAbstract_DestroyBlob(&(element->key));
            RadixAbstract_DestroyBlob(&(element->value));
            free(element);

            if (previous_element)
                previous_element->next_element = next_element;
            else table->first_element = NULL;

            table->length--;
            return true;
        }

        previous_element = RadixTable_KeyIteratorGet(&keys);
    }

    return false;
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
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableElement *element;

    while ((element = RadixTable_KeyIteratorGet(&keys)))
    {
        if (RadixAbstract_BlobEquals(&value, &(element->value)))
            return &(element->key);

        RadixTable_KeyIteratorNext(&keys);
    }

    return NULL;
}

/* Index manipulation */

RadixMemoryBlob * RadixTable_KeyByIndex(
    RadixTable *table,
    unsigned long long index)
{
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableElement *element;

    while ((element = RadixTable_KeyIteratorGet(&keys)))
    {
        if (RadixTable_KeyIteratorIndex(&keys) >= index)
            return &(element->key);

        RadixTable_KeyIteratorNext(&keys);
    }

    return NULL;
}

unsigned long long RadixTable_IndexByKey(
    RadixTable *table,
    RadixMemoryBlob key)
{
    RadixTableKeyIterator keys = RadixTable_NewKeyIterator(table);
    RadixTableElement *element;

    uint64_t hkey = RadixTable_HashKey(key);

    while ((element = RadixTable_KeyIteratorGet(&keys)))
    {
        if (element->keyHash == hkey)
            return RadixTable_KeyIteratorIndex(&keys);
        
        RadixTable_KeyIteratorNext(&keys);
    }

    return 0;
}

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