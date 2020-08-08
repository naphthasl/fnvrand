#include <stdint.h>

#ifndef FNV_HEADER_FILE
#define FNV_HEADER_FILE
    uint32_t fnv1a(const void *data, uint32_t numBytes);
    uint64_t fnv1a64(const void *data, uint32_t numBytes);
    __uint128_t fnv1a128(const void *data, uint32_t numBytes);

    #include "fnv.c"
#endif
