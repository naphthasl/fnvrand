/*
 * @fnv.h
 * Introduces some basic implementations of the Fowler–Noll–Vo hash algorithm,
 * as described here https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
 */

#include <stdint.h>
#include "primitives.h"

#ifndef FNV_HEADER_FILE
#define FNV_HEADER_FILE
    /* These functions are for internal use only. */
    uint32_t fnv1a(const void *data, uint32_t numBytes);
    uint64_t fnv1a64(const void *data, uint32_t numBytes);
    __uint128_t fnv2r128(const void *data, uint32_t numBytes);
    uint64_t fnv2r64(const void *data, uint32_t numBytes);

    /* These functions are intended for public use. */

    /* -- RadixAbstract_fnv1a_64 --
     * Returns an unsigned 64-bit hash of a RadixMemoryBlob, using the standard
     * FNV-1a algorithm.
     */
    uint64_t RadixAbstract_fnv1a_64(RadixMemoryBlob data);

    /* -- RadixAbstract_fnv2r_64 --
     * Returns an unsigned 64-bit hash of a RadixMemoryBlob, using a slightly
     * more advanced approach to FNV, including 128-bit XOR folding and
     * increased input noise.
     */
    uint64_t RadixAbstract_fnv2r_64(RadixMemoryBlob data);

    #include "fnv.c"
#endif
