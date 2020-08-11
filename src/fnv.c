#include <string.h>
#include "primitives.h"
#include "fastrand.h"
#include "fnv.h"

uint32_t fnv1a(const void *data, uint32_t numBytes)
{
    uint32_t hash = 0x811C9DC5;
    const uint8_t *ptr = (const uint8_t *)data;
    
    while (numBytes--) hash = (*ptr++ ^ hash) * 0x01000193;
    return hash;
}

uint64_t fnv1a64(const void *data, uint32_t numBytes)
{
    uint64_t hash = 0xCBF29CE484222325;
    const uint8_t *ptr = (const uint8_t *)data;
    
    while (numBytes--) hash = (*ptr++ ^ hash) * 0x00000100000001B3; 
    return hash;
}

__uint128_t fnv2r128(const void *data, uint32_t numBytes)
{
    const uint64_t hashsplit[2] = {0x62B821756295C58D, 0x6C62272E07BB0142};
    const uint64_t prmesplit[2] = {0x000000000000013B, 0x0000000001000000};
    __uint128_t hash; memcpy(&hash, hashsplit, 16);
    __uint128_t prme; memcpy(&prme, prmesplit, 16);

    const uint8_t *ptr = (const uint8_t *)data;
    
    while (numBytes--) hash = (fr_8noise(*ptr++) ^ hash) * prme; 
    return hash;
}

uint64_t fnv2r64(const void *data, uint32_t numBytes)
{
    __uint128_t unfolded = fnv2r128(data, numBytes);
    return ((uint64_t)unfolded) ^ ((uint64_t)(unfolded >> 64));
}

uint64_t RadixAbstract_fnv1a_64(RadixMemoryBlob data)
{ return fnv1a64(data.ptr, data.length); }
uint64_t RadixAbstract_fnv2r_64(RadixMemoryBlob data)
{ return fnv2r64(data.ptr, data.length); }
