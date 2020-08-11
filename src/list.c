#include <stdint.h>
#include "primitives.h"
#include "list.h"

/* Table initialization */

RadixList RadixList_New()
    // This allows for a single shared point for initialization! :)
    { RadixList list = {0, NULL, NULL}; return list; }

/* Property checking */

unsigned long long RadixList_Length(RadixList *list)
    // Ensures the user doesn't need to interact with the struct directly
    { return list->length; }

/* Key iteration */

RadixListIterator RadixList_NewIterator(RadixList *list)
{
    RadixListIterator ki = {
        list, list->first_element, 0, NULL, NULL, false
    };
    
    if (list->first_element) ki.next = ki.element->next_element;

    return ki;
}

RadixListElement * RadixList_IteratorGet(RadixListIterator *ki)
    { return ki->element; }

RadixListElement * RadixList_IteratorGetPrev(RadixListIterator *ki)
    { return ki->previous; }

RadixListElement * RadixList_IteratorGetNext(RadixListIterator *ki)
    { return ki->next; }

unsigned long long RadixList_IteratorIndex(RadixListIterator *ki)
    { return ki->index; }

void RadixList_IteratorNext(RadixListIterator *ki)
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
    while ((result.current = RadixList_IteratorGet(&keys)))
    {   
        if (RadixList_IteratorCheckElement(&keys, &query))
        {
            /* If found, store informaton about the current, next and previous
             * elements.
             */
            result.index.present = BOOLIFY(result.current);
            result.index.index = RadixList_IteratorIndex(&keys);
            result.next = RadixList_IteratorGetNext(&keys);
            result.previous = RadixList_IteratorGetPrev(&keys);
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