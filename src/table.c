#include <stdint.h>
#include <assert.h>
#include "fnv.h"
#include "bool.h"

typedef struct RadixTableElement {
    uint64_t keyHash;
    RadixMemoryBlob key;
    RadixMemoryBlob value;
    RadixTableElement *next_element;
} RadixTableElement;

typedef struct RadixTable {
    unsigned long long length;
    RadixTableElement *first_element;
} RadixTable;

typedef struct RadixTableKeyIterator {
    RadixTable *table;
    RadixTableElement *element;
} RadixTableKeyIterator;

uint64_t RadixTable_HashKey(RadixMemoryBlob key)
{
    return RadixAbstract_fnv2r_64(&key);
}

RadixTable RadixTable_New()
    {RadixTable table = {0, 0}; return table;}

unsigned long long RadixTable_Length(RadixTable *table)
    { return table->length; }

RadixTableKeyIterator RadixTable_NewKeyIterator(RadixTable *table)
    { RadixTableKeyIterator ki = {table, table->first_element}; return ki; }

RadixTableElement * RadixTable_KeyIteratorGet(RadixTableKeyIterator *ki)
    { return ki->element; }

void RadixTable_KeyIteratorNext(RadixTableKeyIterator *ki)
    { if (ki->element) ki->element = ki->element->next_element; }

RadixTableElement * RadixTable_Find(RadixTable *table, RadixMemoryBlob key)
{
    RadixTableElement *element = (table->first_element);

    if (!element)
        return NULL;

    uint64_t hkey = RadixTable_HashKey(key);

    do
    {
        if (element->keyHash == hkey)
            return element;

        element = element->next_element;
    }
    while (element->next_element);

    return NULL;
}

bool RadixTable_In(RadixTable *table, RadixMemoryBlob key)
    { return (RadixTable_Find(table, key)) ? true : false; }

RadixMemoryBlob * RadixTable_GetItem(RadixTable *table, RadixMemoryBlob key)
{
    RadixTableElement *element = RadixTable_Find(table, key);

    assert(element);

    return &(element->value);
}

void RadixTable_SetItem(
    RadixTable *table,
    RadixMemoryBlob key,
    RadixMemoryBlob value)
{
    RadixTableElement *element = RadixTable_Find(table, key);

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
        element->keyHash = RadixTable_HashKey(key);
        element->key = RadixAbstract_MallocCopy(&key);

        table->length++;
    } else {
        RadixAbstract_DestroyBlob(&(element->value));
    }

    element->value = RadixAbstract_MallocCopy(&value);
}

bool RadixTable_ChangeKey(
    RadixTable *table,
    RadixMemoryBlob key,
    RadixMemoryBlob new_key)
{
    RadixTableElement *element = RadixTable_Find(table, key);

    if (!element)
        return false;
    
    RadixAbstract_DestroyBlob(&(element->key));
    element->keyHash = RadixTable_HashKey(new_key);
    element->key = RadixAbstract_MallocCopy(&new_key);

    return true;
}

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