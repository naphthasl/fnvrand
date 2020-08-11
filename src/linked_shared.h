#include "glbl.h"

#ifndef FLSTTBL_HEADER_FILE
#define FLSTTBL_HEADER_FILE
    /* Search by index, key or value */
    enum RadixIteratorQueryFlags {
        QUERY_INDEX = 1,
        QUERY_KEY = 2,
        QUERY_KEY_CONTAINS = 4,
        QUERY_VALUE = 8,
        QUERY_VALUE_CONTAINS = 16
    };

    // A higher level index structure used to check if an index exists
    // while keeping it starting from 0.
    typedef struct RadixIndex {
        bool present;
        unsigned long long index;
        // See RadixList_IndexStructExists and RadixList_IndexStructPosition
    } RadixIndex;

	typedef struct RadixTableElement {
	    // This allows for better iterative performance when searching for keys
	    uint64_t keyHash;
	    // This provides a key/value pair similar to Python's dicts
	    RadixMemoryBlob key;
	    RadixMemoryBlob value;
	    // Since this is based on the concept of a linked list...
	    struct RadixTableElement *next_element;
	} RadixTableElement;

	typedef struct RadixTable {
	    // The length of the table, which changes during SetItem and
        // DestroyItem
	    unsigned long long length;
	    // The very first element of the table, which starts the chain.
	    RadixTableElement *first_element;
	    // Whether or not to insert at the end
	    bool insert_at_end;
	} RadixTable;

    typedef struct RadixListElement {
        // Represents the value in the list, which will also be fully compared
        // when searching for values.
        RadixMemoryBlob value;
        // Points to the next element in the list, as this is a linked list
        struct RadixListElement *next_element;
    } RadixListElement;

    // Root definition of the list, containing the length, a pointer to the
    // first element and a pointer to the last element.
    typedef struct RadixList {
        unsigned long long length;
        RadixListElement *first_element;
        RadixListElement *last_element;
    } RadixList;

    // Macros to get RadixIndex values. Not strictly neccessary - just provides
    // an alternative access method.
    #define R_IndexPresent(a) (a.present)
    #define R_IndexPosition(a) (a.index)

    // Macros that apply to both iterators
    #define RadixIterator_Get(a) ((a)->element)
    #define RadixIterator_GetPrev(a) ((a)->previous)
    #define RadixIterator_GetNext(a) ((a)->next)
    #define RadixIterator_GetIndex(a) ((a)->index)

    // Property checking macros
    #define RadixLL_Length(a) ((a)->length)

    #define KEYITERATOR_STRUCT(name, type, elementtype)\
	typedef struct name {\
	    type *table;\
	    elementtype *element;\
	    unsigned long long index;\
	    elementtype *previous;\
	    elementtype *next;\
	    bool exhausted;\
	} name;
    KEYITERATOR_STRUCT(RadixTableKeyIterator, RadixTable, RadixTableElement)
    KEYITERATOR_STRUCT(RadixListIterator, RadixList, RadixListElement)
    #undef KEYITERATOR_STRUCT

    #define NEW_KEYITERATOR_FUNC(name, type, type2)\
    type name(type2 *table)\
    {\
        type ki = {\
            table, table->first_element, 0, NULL, NULL, false\
        };\
        if (table->first_element) ki.next = ki.element->next_element;\
        return ki;\
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

    #define KEYITERATOR_FUNC(name, type)\
    void name(type ki)\
    {\
        if (ki->element)\
        {\
            ki->previous = ki->element;\
            ki->element = ki->element->next_element;\
            if (ki->element) ki->next = ki->element->next_element;\
            else ki->next = NULL;\
            \
            ki->index++;\
        } else {\
            ki->exhausted = true;\
        }\
    }
    #ifdef FTABL_HEADER_FILE
        KEYITERATOR_FUNC(RadixTable_KeyIteratorNext, RadixTableKeyIterator *)
    #endif
    #ifdef FLIST_HEADER_FILE
        KEYITERATOR_FUNC(RadixList_IteratorNext,  RadixListIterator *)
    #endif
    #undef KEYITERATOR_FUNC
#endif