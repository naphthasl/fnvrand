#include "glbl.h"
#include "table.h"
#include "bool.h"
#include "primitives.h"
#include "registry.h"

R_registry R_registry_new()
    { R_registry temp = {RadixTable_New()}; return temp; }

bool R_registry_in(R_registry *registry, RadixMemoryBlob key)
    { return RadixTable_In(&registry->tab, key); }

void R_registry_set(R_registry *registry, RadixMemoryBlob key, void *value)
{
    RadixTable_SetItem(&registry->tab, key,
        RadixAbstract_ConstructPointerBlob(&value, sizeof(void*)));
}

void ** R_registry_get(R_registry *registry, RadixMemoryBlob key)
{
    return RadixAbstract_GetBlobPointer(
        *RadixTable_GetItem(&registry->tab, key));
}

void R_registry_del(R_registry *registry, RadixMemoryBlob key)
    { RadixTable_DestroyItem(&registry->tab, key); }

void R_registry_destroy(R_registry *registry)
    { RadixTable_DestroyTable(&registry->tab); }