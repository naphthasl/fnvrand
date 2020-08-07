#ifndef FNV_HEADER_FILE
#define FNV_HEADER_FILE

#include <stdint.h>

uint32_t fnv1a(const void *data, uint32_t numBytes);
uint64_t fnv1a64(const void *data, uint64_t numBytes);

#include "fnv.c"
#endif
