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
#endif