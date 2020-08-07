#include "fnv.h"

unsigned long long cr_keygen(
    const void *key,
    unsigned int numBytesKey,
    unsigned int iterations)
{
    unsigned long long pivot = fnv1a64(key, numBytesKey);

    register unsigned int i;
    for (i = 0; i < iterations; i++)
        pivot = fnv1a64(&pivot, sizeof(unsigned long long));

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
    register unsigned long long hkey = cr_keygen(key, numBytesKey, 16384);
    register unsigned long long fkey;

    unsigned int index;

    while (numBytesDest--)
    {
        index = ((original_length - numBytesDest) - 1);
        if (!(index % sizeof(unsigned long long))) {
            fkey = (hkey ^ fnv1a64(
                &index,
                sizeof(unsigned long long)));
        } else {
            fkey >>= 1;
        }
        *dcstream ^= fkey;
        dcstream++;
    }
}