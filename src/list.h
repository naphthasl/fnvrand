#include "glbl.h"
#include "primitives.h"

#ifndef FLIST_HEADER_FILE
#define FLIST_HEADER_FILE
    typedef struct RadixListElement {
        RadixMemoryBlob value;
        struct RadixListElement *next_element;
    } RadixListElement;

    typedef struct RadixList {
        unsigned long long length;
        RadixListElement *first_element;
        RadixListElement *last_element;
    } RadixList;

    typedef struct RadixListIndex {
        bool present;
        unsigned long long index;
    } RadixListIndex;

    typedef struct RadixListIterator {
        RadixList *list;
        RadixListElement *element;
        unsigned long long index;
        RadixListElement *previous;
        RadixListElement *next;
        bool exhausted;
    } RadixListIterator;

    typedef struct RadixListQueryResult {
        bool found;
        RadixListIndex index;
        RadixListElement *previous;
        RadixListElement *current;
        RadixListElement *next;
    } RadixListQueryResult;

    typedef struct RadixListQuery {
        char query_for;
        RadixMemoryBlob *value;
        unsigned long long index;
        unsigned int containsInterval;
    } RadixListQuery;

    // IMPORTANT: Read this for query flags
    #include "linked_shared.h"
#endif
