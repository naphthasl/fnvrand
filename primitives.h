#ifndef FPRIM_HEADER_FILE
#define FPRIM_HEADER_FILE
    typedef struct {
        unsigned long long length;
        void *ptr;
    } RadixMemoryBlob;

    #include "primitives.c"
#endif