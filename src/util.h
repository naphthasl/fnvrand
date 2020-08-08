#include <stdint.h>

#ifndef FUTIL_HEADER_FILE
#define FUTIL_HEADER_FILE
    #define true 1
    #define false 0
    #define bool char

    void u_memtohex(void *dest, const void *src, register uint64_t numSrcBytes);
    void u_printmemhex(const void *src, register uint64_t numSrcBytes);

    #include "util.c"
#endif