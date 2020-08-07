#ifndef FNV_HEADER_FILE
#define FNV_HEADER_FILE

unsigned int fnv1a(const void* data, register unsigned int numBytes);
unsigned int fnv1r(const void *data, register unsigned int numBytes);

#include "fnv.c"
#endif
