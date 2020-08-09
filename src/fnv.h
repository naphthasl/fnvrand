#include <stdint.h>
#include "primitives.h"

#ifndef FNV_HEADER_FILE
#define FNV_HEADER_FILE
    uint32_t fnv1a(const void *data, uint32_t numBytes);
    uint64_t fnv1a64(const void *data, uint32_t numBytes);
    __uint128_t fnv2r128(const void *data, uint32_t numBytes);
    uint64_t fnv2r64(const void *data, uint32_t numBytes);
    uint64_t RadixAbstract_fnv1a_64(RadixMemoryBlob *data);
    uint64_t RadixAbstract_fnv2r_64(RadixMemoryBlob *data);

    #include "fnv.c"
#endif
