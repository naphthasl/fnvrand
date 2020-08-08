#include <stdint.h>

#ifndef FASTRAND_HEADER_FILE
#define FASTRAND_HEADER_FILE
    uint64_t fr_genkeystream(
        void *dest,
        register uint64_t seed,
        register uint32_t numBytes);

    uint64_t fr_64noise(uint64_t seed);
    uint8_t  fr_8noise (uint8_t  seed);

    #include "fastrand.c"
#endif
