#include "fastrand.h"

unsigned int fnv1a(const void *data, register unsigned int numBytes)
{
    register unsigned int hash = 0x811C9DC5;
    const unsigned char *ptr = (const unsigned char *)data;
    
    if (!data)
        return 0;
    
    while (numBytes--)
        hash = (*ptr++ ^ hash) * 0x01000193;
        
    return hash;
}

unsigned int fnv1r(const void *data, register unsigned int numBytes)
{
    register unsigned int hash = fnv1a(data, numBytes);
    
    return hash ^ fr_randint(hash ^ numBytes);
}
