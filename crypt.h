#include <stdint.h>

#ifndef FCRYPT_HEADER_FILE
#define FCRYPT_HEADER_FILE
    typedef struct {
        uint64_t gkey, fkey;
        uint32_t index;
    } cr_state;

    uint64_t cr_keygen(
        const void *key,
        uint32_t numBytesKey,
        uint64_t seed,
        register uint32_t iterations);

    void cr_init_crypt(cr_state *state, const uint64_t gkey);
    uint8_t cr_operate_byte(uint8_t i, cr_state *state);
    void cr_encrypt_decrypt(void *dest, uint32_t numBytes, const uint64_t gkey);

    #include "crypt.c"
#endif
