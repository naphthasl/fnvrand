/*
 * @fastrand.h
 * Allows for the generation of random integers and blobs.
 */

#include <stdint.h>

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

    /* -- RadixAbstract_RandomBlob --
     * Returns a new blob containing completely random data, taking the blob
     * length and a seed to use for random number generation as an argument.
     * 
     * This function mallocates a blob of given length.
     * 
     * Do NOT forget to destroy it with RadixAbstract_DestroyBlob when you are
     * done.
     */
    RadixMemoryBlob RadixAbstract_RandomBlob(uint32_t length, uint64_t seed);

    /* -- fr_uniquerandom --
     * Returns a completely unique random integer (no seed) using rand() in
     * the range [0, n]. Can be used to seed the random number/stream
     * generators above.
     * 
     * n must be smaller than MAX_RAND.
     */
    int fr_uniquerandom(int n);

    #include "fastrand.c"
#endif
