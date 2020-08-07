#ifndef FCRYPT_HEADER_FILE
#define FCRYPT_HEADER_FILE

unsigned long long cr_keygen(
    const void *key,
    unsigned int numBytesKey,
    unsigned int iterations);

void cr_encrypt_decrypt(
    void *dest,
    unsigned int numBytesDest,
    const void *key,
    unsigned int numBytesKey);

#include "crypt.c"
#endif
