RadixMemoryBlob RadixAbstract_ConstructCOWBlob(void *dest, unsigned long long length)
{
    RadixMemoryBlob final = {length, dest}; return final;
}

RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length)
{
    return RadixAbstract_ConstructCOWBlob(malloc(length), length);
}

void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob)
{
    free(blob->ptr);
}