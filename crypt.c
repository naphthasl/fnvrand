#include "fnv.h"
#include "fastrand.h"

uint64_t cr_keygen(
    const void *key,
    uint32_t numBytesKey,
    uint64_t seed,
    uint32_t iterations)
{
    uint64_t pivot = fnv1a64(key, numBytesKey) ^ seed;
    while (iterations--) pivot ^= fnv1a64(&pivot, sizeof(uint64_t));
    return pivot;
}

void cr_encrypt_decrypt(
    void *dest,
    uint32_t numBytesDest,
    const uint64_t gkey)
{
    uint8_t *dcstream = (uint8_t *)dest;

    uint32_t original_length = numBytesDest;

    uint64_t fkey = 0;
    while (numBytesDest--)
    {
        if (!(((original_length - numBytesDest) - 1) % sizeof(uint64_t)))
        {
            fkey ^= gkey ^ fnv1a64(&numBytesDest, sizeof(uint32_t));
        } else {
            fkey >>= sizeof(uint8_t);
        }

        *dcstream++ ^= (uint8_t)fkey;
    }
}