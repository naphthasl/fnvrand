#ifndef FASTRAND_HEADER_FILE
#define FASTRAND_HEADER_FILE

unsigned int fr_genkeystream(
    void *dest,
    register unsigned int seed,
    unsigned int len);

unsigned int fr_randint(register unsigned int seed);

#include "fastrand.c"
#endif