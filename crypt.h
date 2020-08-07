#include <stdint.h>

#ifndef FCRYPT_HEADER_FILE
#define FCRYPT_HEADER_FILE
    #ifndef CR_KEY_ITERATIONS
    #define CR_KEY_ITERATIONS 16384
    #endif

    uint64_t cr_keygen(
        const void *key,
        uint32_t numBytesKey,
        uint32_t iterations);

    void cr_encrypt_decrypt(
        void *dest,
        uint32_t numBytesDest,
        const void *key,
        uint32_t numBytesKey);

    #include "crypt.c"
#endif
