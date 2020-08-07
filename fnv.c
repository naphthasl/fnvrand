#include "fastrand.h"

unsigned int fnv1a(const void *data, register unsigned int numBytes)
{
    register unsigned int hash = 0x811C9DC5;
    const unsigned char *ptr = (const unsigned char *)data;
    
    if (data)
    {
        while (numBytes--)
            hash = (*ptr++ ^ hash) * 0x01000193;
    }

    return hash;
}

unsigned long long fnv1a64(
    const void *data,
    register unsigned long long numBytes)
{
    register unsigned long long hash = 0xCBF29CE484222325;
    const unsigned char *ptr = (const unsigned char *)data;
    
    if (data)
    {
        while (numBytes--)
            hash = (*ptr++ ^ hash) * 0x00000100000001B3;
    }
        
    return hash;
}

unsigned int fnv1r(const void *data, register unsigned int numBytes)
{
    register unsigned int hash = fnv1a(data, numBytes);
    
    return hash ^ fr_randint(hash ^ numBytes);
}
