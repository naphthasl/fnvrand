#ifndef FASTRAND_HEADER_FILE
#define FASTRAND_HEADER_FILE

#include <stdint.h>

uint32_t fr_genkeystream(
    void *dest,
    uint32_t seed,
    uint32_t numBytes);

uint32_t fr_randint(uint32_t seed);
uint64_t fr_rand64(uint64_t seed);

#include "fastrand.c"
#endif
