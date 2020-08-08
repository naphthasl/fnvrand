#include "fnv.h"
#include "fastrand.h"

uint64_t cr_keygen(
    const void *key,
    uint32_t numBytesKey,
    uint64_t seed,
    register uint32_t iterations)
{
    uint64_t pivot = fnv1a64(key, numBytesKey) ^ seed;
    while (iterations--) pivot ^= fnv1a64(&pivot, 8);
    return pivot;
}

void cr_init_crypt(cr_state *state, const uint64_t gkey)
{
    state->gkey = gkey; state->fkey = 0; state->index = 0;
}

uint8_t cr_operate_byte(uint8_t i, cr_state *state)
{
    if (!(state->index % 8))
    {
        state->fkey ^= state->gkey ^ fnv1a64(&(state->index), 4);
    } else { state->fkey = ~(state->fkey >> 8); }
    state->index++;

    return i ^= (uint8_t)fr_8noise(state->fkey);
}

void cr_encrypt_decrypt(void *dest, uint32_t numBytes, const uint64_t gkey)
{
    register uint8_t *dcstream = (uint8_t *)dest;
    cr_state state; cr_init_crypt(&state, gkey);
    while (numBytes--) { *dcstream++ = cr_operate_byte(*dcstream, &state); }
}