#include "fnv.h"
#include "fastrand.h"

#ifndef CR_KEY_ITERATIONS
#define CR_KEY_ITERATIONS 16384
#endif

#define KEY_WIDTH sizeof(unsigned long long)

unsigned long long cr_keygen(
    const void *key,
    unsigned int numBytesKey,
    register unsigned int iterations)
{
    unsigned long long pivot = fnv1a64(key, numBytesKey);
    while (iterations--) pivot = fnv1a64(&pivot, KEY_WIDTH);
    return pivot;
}

void cr_encrypt_decrypt(
    void *dest,
    unsigned int numBytesDest,
    const void *key,
    unsigned int numBytesKey)
{
    register unsigned int original_length = numBytesDest;
    register unsigned char *dcstream = dest;
    register unsigned long long hkey = cr_keygen(
        key, numBytesKey, CR_KEY_ITERATIONS);
    register unsigned long long fkey = 0;

    while (numBytesDest--)
    {
        if (!(((original_length - numBytesDest) - 1) % KEY_WIDTH))
        {
            fkey = fr_rand64(hkey ^ fnv1a64(&numBytesDest, KEY_WIDTH));
        } else {
            fkey >>= 1;
        }

        *dcstream++ ^= (unsigned char)fkey;
    }
}