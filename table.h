#include "primitives.h"

#ifndef FLIST_HEADER_FILE
#define FLIST_HEADER_FILE
    typedef struct RadixTableElement {
        RadixMemoryBlob key;
        RadixMemoryBlob value;
    } RadixTableElement;

    typedef struct RadixTable {
        unsigned int length;
        RadixTableElement *elements;
    } RadixTable;

    #include "table.c"
#endif