/*
 * @crypt.h
 * Implements an insecure but extremely fast encryption algorithm, capable of
 * both block and stream encryption.
 */

#include <stdint.h>
#include "primitives.h"

#ifndef FCRYPT_HEADER_FILE
#define FCRYPT_HEADER_FILE
    /* -- cr_state --
     * This is a dummy typedef for the cr_state structure, which you'll need to
     * use in order to perform stream encryption.
     */
    typedef struct cr_state cr_state;

    /* -- cr_keygen --
     * Allows you to securely-ish generate a key for the encryption algorithm.
     * Return a uint64_t containing the 64-bit key. Takes a blob as input, so
     * you can turn any memory range into the key. Can be seeded in order to
     * use initialization vectors. Allows you to specify an amount of
     * iterations. Recommended iteration count is 16777216.
     */
    uint64_t cr_keygen(
        RadixMemoryBlob key,
        uint64_t seed,
        register uint32_t iterations);

    /* -- cr_init_crypt --
     * Creates a brand new cr_state for encrypting per-byte or per-blob later.
     * Takes a key generated with cr_keygen as the argument.
     */
    cr_state cr_init_crypt(const uint64_t gkey);

    /* -- cr_operate_byte --
     * Encrypts a single byte using a given cr_state. Can be used for stream
     * encryption. Also decrypts.
     */
    uint8_t cr_operate_byte(uint8_t i, cr_state *state);

    /* -- cr_encrypt_decrypt --
     * Encrypts or decrypts a given block in-place. Requires a key generated
     * with cr_keygen.
     */
    void cr_encrypt_decrypt(RadixMemoryBlob data, const uint64_t gkey);

    #include "crypt.c"
#endif
