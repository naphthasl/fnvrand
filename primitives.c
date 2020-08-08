#include "util.h"

typedef struct RadixMemoryBlob {
    unsigned long long length;
    void *ptr;
    bool heap;
} RadixMemoryBlob;

RadixMemoryBlob RadixAbstract_ConstructCOWBlob(void *dest, unsigned long long length)
{
    RadixMemoryBlob final = {length, dest, false}; return final;
}

RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length)
{
    RadixMemoryBlob temp = RadixAbstract_ConstructCOWBlob(malloc(length), length);
    temp.heap = true;
    return temp;
}

unsigned long long RadixAbstract_GetBlobLength(RadixMemoryBlob *blob)
{
    return blob->length;
}

void * RadixAbstract_GetBlobPointer(RadixMemoryBlob *blob)
{
    return blob->ptr;
}

void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob)
{
    if (blob->heap) {
        free(blob->ptr);
    }
}