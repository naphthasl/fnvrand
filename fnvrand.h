#ifndef FNVRAND_HEADER_FILE
#define FNVRAND_HEADER_FILE

unsigned int fnv_genkeystream(
    register unsigned char *dest,
    register unsigned int seed,
    unsigned int len);

unsigned int fnv_randint(register unsigned int seed);
unsigned int fnv1a(const void* data, register unsigned int numBytes);
unsigned int fnv1r(const void* data, unsigned int numBytes);

#include "fnvrand.c"
#endif
