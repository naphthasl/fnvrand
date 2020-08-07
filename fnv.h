#ifndef FNV_HEADER_FILE
#define FNV_HEADER_FILE

unsigned int fnv1a(const void *data, unsigned int numBytes);
unsigned long long fnv1a64(
    const void *data,
    unsigned long long numBytes);

#include "fnv.c"
#endif
