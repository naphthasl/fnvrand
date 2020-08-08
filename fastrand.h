#include <stdint.h>

#ifndef FASTRAND_HEADER_FILE
#define FASTRAND_HEADER_FILE
    uint64_t fr_genkeystream(
        void *dest,
        uint64_t seed,
        uint32_t numBytes);

    uint64_t fr_64noise(uint64_t seed);
    uint32_t fr_32noise(uint32_t seed);
    uint16_t fr_16noise(uint16_t seed);
    uint8_t  fr_8noise (uint8_t  seed);

    #include "fastrand.c"
#endif
