#include <stdint.h>

#ifndef FUTIL_HEADER_FILE
#define FUTIL_HEADER_FILE
    void u_memtohex(void *dest, const void *src, uint64_t numSrcBytes);
    void u_printmemhex(const void *src, uint64_t numSrcBytes);

    #include "util.c"
#endif