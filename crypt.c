#include "fnv.h"
#include "fastrand.h"

#ifndef CR_KEY_ITERATIONS
#define CR_KEY_ITERATIONS 16384
#endif

#define KEY_WIDTH sizeof(uint64_t)

uint64_t cr_keygen(
    const void *key,
    uint32_t numBytesKey,
    uint32_t iterations)
{
    uint64_t pivot = fnv1a64(key, numBytesKey);
    while (iterations--) pivot = fnv1a64(&pivot, KEY_WIDTH);
    return pivot;
}

void cr_encrypt_decrypt(
    void *dest,
    uint32_t numBytesDest,
    const void *key,
    uint32_t numBytesKey)
{
    uint32_t original_length = numBytesDest;
    uint8_t *dcstream = dest;
    uint64_t hkey = cr_keygen(
        key, numBytesKey, CR_KEY_ITERATIONS);
    uint64_t fkey = 0;

    while (numBytesDest--)
    {
        if (!(((original_length - numBytesDest) - 1) % KEY_WIDTH))
        {
            fkey = fr_rand64(hkey ^ fnv1a64(&numBytesDest, KEY_WIDTH));
        } else {
            fkey >>= 1;
        }

        *dcstream++ ^= (uint8_t)fkey;
    }
}