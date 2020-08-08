#include <stdint.h>

#ifndef FCRYPT_HEADER_FILE
#define FCRYPT_HEADER_FILE
    uint64_t cr_keygen(
        const void *key,
        uint32_t numBytesKey,
        uint64_t seed,
        register uint32_t iterations);

    void cr_encrypt_decrypt(
        void *dest,
        uint32_t numBytesDest,
        uint64_t gkey);

    #include "crypt.c"
#endif
