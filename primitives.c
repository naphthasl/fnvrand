typedef struct RadixMemoryBlob {
    unsigned long long length;
    void *ptr;
} RadixMemoryBlob;

RadixMemoryBlob RadixAbstract_ConstructCOWBlob(void *dest, unsigned long long length)
{
    RadixMemoryBlob final = {length, dest}; return final;
}

RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length)
{
    return RadixAbstract_ConstructCOWBlob(malloc(length), length);
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
    free(blob->ptr);
}