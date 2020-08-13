#include "glbl.h"
#include "bool.h"
#include "primitives.h"
#include "table.h"

#ifndef RAREG_HEADER_FILE
#define RAREG_HEADER_FILE
    typedef struct R_registry {
        RadixTable tab;
        // More to be added here if needed
    } R_registry;

    R_registry R_registry_new();
    bool R_registry_in(R_registry *registry, RadixMemoryBlob key);
    void R_registry_set(
        R_registry *registry,
        RadixMemoryBlob key,
        void *value);
    void ** R_registry_get(R_registry *registry, RadixMemoryBlob key);
    void R_registry_del(R_registry *registry, RadixMemoryBlob key);
    void R_registry_destroy(R_registry *registry);
#endif