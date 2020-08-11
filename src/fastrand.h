/*
 * @fastrand.h
 * Allows for the generation of random integers and blobs.
 */

#include <stdint.h>
#include "primitives.h"

#ifndef FASTRAND_HEADER_FILE
#define FASTRAND_HEADER_FILE
    /* These functions are for internal use only. */
    uint64_t fr_genkeystream(
        void *dest,
        register uint64_t seed,
        register uint32_t numBytes);

    /* These functions are intended for public use. */

    /* -- fr_64noise --
     * Returns a 64-bit unsigned random integer. Takes a seed argument.
     */
    uint64_t fr_64noise(uint64_t seed);

    /* -- fr_8noise --
     * Returns a 8-bit unsigned random integer. Takes a seed argument.
     */
    uint8_t  fr_8noise (uint8_t  seed);

    /* -- RadixAbstract_RandomizeBlob --
     * Fills a blob with random data in-place. Can take a seed as an argument.
     * Does not create a new blob, original data will be overwritten.
     */
    void RadixAbstract_RandomizeBlob(RadixMemoryBlob blob, uint64_t seed);

    /* -- fr_uniquerandom --
     * Returns a completely unique random integer (no seed) using rand() in
     * the range [0, n]. Can be used to seed the random number/stream
     * generators above.
     * 
     * n must be smaller than MAX_RAND.
     */
    int fr_uniquerandom(int n);

#endif
