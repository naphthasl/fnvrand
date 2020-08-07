#ifndef FASTRAND_HEADER_FILE
#define FASTRAND_HEADER_FILE

unsigned int fr_genkeystream(
    register unsigned int *dest,
    register unsigned int seed,
    unsigned int len);

unsigned int fr_randint(register unsigned int seed);

#include "fastrand.c"
#endif
