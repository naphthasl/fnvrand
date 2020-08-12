#include "linked_shared.h"
#include "bool.h"

#define NEW_KEYITERATOR_FUNC(name, type, type2)                         \
type name(type2 *table)                                                 \
{                                                                       \
    type ki = {                                                         \
        table, table->first_element, 0, NULL, NULL, false               \
    };                                                                  \
    if (table->first_element) ki.next = ki.element->next_element;       \
    return ki;                                                          \
}
#ifdef FTABL_HEADER_FILE
    NEW_KEYITERATOR_FUNC(
        RadixTable_NewKeyIterator, RadixTableKeyIterator, RadixTable)
#endif
#ifdef FLIST_HEADER_FILE
    NEW_KEYITERATOR_FUNC(
        RadixList_NewIterator, RadixListIterator, RadixList)
#endif
#undef NEW_KEYITERATOR_FUNC

#define KEYITERATOR_FUNC(name, type)                                    \
void name(type ki)                                                      \
{                                                                       \
    if (ki->element)                                                    \
    {                                                                   \
        ki->previous = ki->element;                                     \
        ki->element = ki->element->next_element;                        \
        if (ki->element) ki->next = ki->element->next_element;          \
        else ki->next = NULL;                                           \
        ki->index++;                                                    \
    } else {                                                            \
        ki->exhausted = true;                                           \
    }                                                                   \
}
#ifdef FTABL_HEADER_FILE
    KEYITERATOR_FUNC(RadixTable_KeyIteratorNext, RadixTableKeyIterator *)
#endif
#ifdef FLIST_HEADER_FILE
    KEYITERATOR_FUNC(RadixList_IteratorNext,  RadixListIterator *)
#endif
#undef KEYITERATOR_FUNC