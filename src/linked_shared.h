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

    // Macros to get RadixIndex values. Not strictly neccessary - just provides
    // an alternative access method.
    #define R_IndexPresent(a) (a.present)
    #define R_IndexPosition(a) (a.index)
#endif