#ifndef FPRIM_HEADER_FILE
#define FPRIM_HEADER_FILE
    typedef struct RadixMemoryBlob RadixMemoryBlob;

    RadixMemoryBlob RadixAbstract_ConstructCOWBlob(
        void *dest,
        unsigned long long length);
    RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length);
    RadixMemoryBlob RadixAbstract_SliceBlob(
        RadixMemoryBlob *blob,
        unsigned long long offset,
        unsigned long long length);
    void RadixAbstract_InsertBlob(
        RadixMemoryBlob *source,
        RadixMemoryBlob *target,
        unsigned long long target_offset);
    unsigned long long RadixAbstract_GetBlobLength(RadixMemoryBlob *blob);
    void * RadixAbstract_GetBlobPointer(RadixMemoryBlob *blob);
    void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob);

    #include "primitives.c"
#endif