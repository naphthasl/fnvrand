#include "glbl.h"
#include "primitives.h"
#include "list.h"
#include "bool.h"
#include "macros.h"

// IMPORTANT: Read this for query flags
#include "linked_shared.h"

/* Table initialization */

RadixList RadixList_New()
    // This allows for a single shared point for initialization! :)
    { RadixList list = {0, NULL, NULL}; return list; }

/* Key iteration */

bool RadixList_IteratorCheckElement(
    RadixListIterator *ki,
    RadixListQuery *kq)
{
    if (kq->query_for & QUERY_INDEX)
        if (ki->index == kq->index && ki->element) return true;
    if (kq->query_for & QUERY_VALUE)
        if (RadixAbstract_BlobEquals(ki->element->value, *kq->value))
            return true;
    if (kq->query_for & QUERY_VALUE_CONTAINS)
        if (RadixAbstract_BlobContains
            (ki->element->value, *kq->value, kq->containsInterval))
            return true;

    return false;
}

RadixListQuery RadixList_ConstructQuery(
    char query_for,
    RadixMemoryBlob *value,
    unsigned long long index,
    unsigned int containsInterval)
{
    RadixListQuery query = {
        query_for, value, index, containsInterval
    };

    return query;
}

RadixListQueryResult RadixList_Query(
    RadixList *list,
    RadixListQuery query)
{
    // Begin the key iterator.
    RadixListIterator keys = RadixList_NewIterator(list);
    RadixListQueryResult result;
    result.found = false;

    // Check if the current element is not d e a d each iteration.
    while ((result.current = RadixIterator_Get(&keys)))
    {   
        if (RadixList_IteratorCheckElement(&keys, &query))
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

        RadixList_IteratorNext(&keys);
    }

    if (!result.found)
    {
        result.current = NULL;
        result.index.present = false;
    }

    return result;
}

RadixListElement * RadixList_FindIndex(
    RadixList *list,
    unsigned long long index)
{
    RadixListQuery query = RadixList_ConstructQuery(
        QUERY_INDEX, NULL, index, 1);
    return RadixList_Query(list, query).current;
}

RadixListElement * RadixList_FindValue(
    RadixList *list,
    RadixMemoryBlob value)
{
    RadixListQuery query = RadixList_ConstructQuery(
        QUERY_VALUE, &value, 0, 1);
    return RadixList_Query(list, query).current;
}

bool RadixList_IndexIn(RadixList *list, unsigned long long index)
    { return BOOLIFY(RadixList_FindIndex(list, index)); }

bool RadixList_ValueIn(RadixList *list, RadixMemoryBlob value)
    { return BOOLIFY(RadixList_FindValue(list, value)); }

RadixMemoryBlob * RadixList_GetValue(RadixList *list, unsigned long long index)
    { return &(RadixList_FindIndex(list, index)->value); }

RadixIndex RadixList_GetIndex(RadixList *list, RadixMemoryBlob value)
{
    RadixListQuery query = RadixList_ConstructQuery(
        QUERY_VALUE, &value, 0, 1);
    return RadixList_Query(list, query).index;
}

bool RadixList_SetIndex(
    RadixList *list,
    unsigned long long index,
    RadixMemoryBlob value)
{
    RadixListElement *element = RadixList_FindIndex(list, index);

    // If the element doesn't exist, return false
    if (!element) return false;

    // Malloc a new blob for the value to isolate the table.
    RadixAbstract_DestroyBlob(&(element->value));
    element->value = RadixAbstract_MallocCopy(value);

    // If the value was set properly, return true
    return true;
}

unsigned long long RadixList_InsertNew(
    RadixList *list,
    unsigned long long new_position)
{
    RadixListElement **target;
    bool modified_last = false;

    new_position = MIN(new_position, list->length);

    if (!new_position) target = &(list->first_element);
    else if (new_position == list->length)
        target = &(list->last_element->next_element), modified_last = true;
    else target = &(RadixList_FindIndex(list, new_position - 1)->next_element);

    if (!list->first_element) modified_last = true;

    RadixListElement *old_target = *target;
    (*target) = malloc(sizeof(RadixListElement));
    (*target)->next_element = old_target;

    list->length++;

    if (modified_last) list->last_element = *target;

    return new_position;
}

unsigned long long RadixList_Append(RadixList *list, char side)
    { return RadixList_InsertNew(list, (side ? list->length : 0)); }

unsigned long long RadixList_AppendValue(
    RadixList *list,
    char side,
    RadixMemoryBlob value)
{
    unsigned long long index = RadixList_Append(list, side);
    RadixList_SetIndex(list, index, value);

    return index;
}

void RadixList_SwapIndexValues(
    RadixList *list,
    unsigned long long p0,
    unsigned long long p1)
{
    RadixMemoryBlob temp = RadixList_FindIndex(list, p1)->value;
    RadixList_FindIndex(list, p1)->value = RadixList_FindIndex(
        list, p0
    )->value;
    RadixList_FindIndex(list, p0)->value = temp;
}

bool RadixList_DestroyIndex(RadixList *list, unsigned long long index)
{
    RadixListQuery query_in = RadixList_ConstructQuery(
        QUERY_INDEX, NULL, index, 1);
    RadixListQueryResult query = RadixList_Query(list, query_in);

    if (!query.found)
        return false;

    if (query.previous)
        query.previous->next_element = query.next;
    else list->first_element = query.next;

    RadixAbstract_DestroyBlob(&(query.current->value));
    free(query.current);

    list->length--;

    return true;
}

void RadixList_DestroyList(RadixList *list)
{
    RadixListIterator keys = RadixList_NewIterator(list);
    RadixListElement *element;

    while ((element = RadixIterator_Get(&keys)))
    {
        RadixAbstract_DestroyBlob(&(element->value));
        free(element);

        RadixList_IteratorNext(&keys);
    }

    list->first_element = NULL;
    list->length = 0;
}

void RadixList_Concatenate(RadixList *dest, RadixList *src, char side)
{
    RadixListIterator keys = RadixList_NewIterator(src);
    RadixListElement *element;
    unsigned long long new_index;

    while ((element = RadixIterator_Get(&keys)))
    {
        (void)RadixList_AppendValue(dest, side, element->value);

        RadixList_IteratorNext(&keys);
    }
}
