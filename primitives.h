#ifndef FPRIM_HEADER_FILE
#define FPRIM_HEADER_FILE
    typedef struct {
        unsigned long long length;
        void *ptr;
    } RadixMemoryBlob;

    RadixMemoryBlob RadixAbstract_ConstructCOWBlob(void *dest, unsigned long long length);
    RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length);
    void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob);

    #include "primitives.c"
#endif