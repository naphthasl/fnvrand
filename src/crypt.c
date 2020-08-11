#include "glbl.h"
#include "fnv.h"
#include "fastrand.h"
#include "primitives.h"
#include "crypt.h"

typedef struct cr_state {
    uint64_t gkey, fkey;
    uint32_t index;
} cr_state;

uint64_t cr_keygen(
    RadixMemoryBlob key,
    uint64_t seed,
    register uint32_t iterations)
{
    uint64_t pivot = fnv2r64(
        RadixAbstract_GetBlobPointer(key),
        RadixAbstract_GetBlobLength(key)
    ) ^ seed;
    while (iterations--) pivot ^= fnv2r64(&pivot, 8);
    return pivot;
}

cr_state cr_init_crypt(const uint64_t gkey)
{
    cr_state state = {gkey, 0, 0};
    return state;
}

uint8_t cr_operate_byte(uint8_t i, cr_state *state)
{
    if (!(state->index % 8))
        state->fkey ^= state->gkey ^ fnv1a64(&(state->index), 4);
    else state->fkey = ~(state->fkey >> 8);

    state->index++;
    return i ^= (uint8_t)fr_8noise(state->fkey);
}

void cr_encrypt_decrypt(RadixMemoryBlob data, const uint64_t gkey)
{
    register uint32_t numBytes = RadixAbstract_GetBlobLength(data);
    register uint8_t *dcstream = (uint8_t *)RadixAbstract_GetBlobPointer(data);
    cr_state state = cr_init_crypt(gkey);
    while (numBytes--)
        { *dcstream = cr_operate_byte(*dcstream, &state); dcstream++; }
}
