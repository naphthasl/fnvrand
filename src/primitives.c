#include <assert.h>
#include "bool.h"

typedef struct RadixMemoryBlob {
    unsigned long long length;
    void *ptr;
    bool heap;
    bool destroyed;
} RadixMemoryBlob;

RadixMemoryBlob RadixAbstract_ConstructPointerBlob(
    void *dest,
    unsigned long long length)
{
    RadixMemoryBlob final = {length, dest, false, false};
    return final;
}

RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length)
{
    RadixMemoryBlob temp = RadixAbstract_ConstructPointerBlob(
        malloc(length), length);
    temp.heap = true;
    return temp;
}

void RadixAbstract_ResizeMallocBlob(
    RadixMemoryBlob *blob,
    unsigned long long new_length)
{
    assert(blob->heap);
    blob->ptr = realloc(blob->ptr, new_length);
}

RadixMemoryBlob RadixAbstract_SliceBlob(
    RadixMemoryBlob blob,
    unsigned long long offset,
    unsigned long long length)
{
    RadixMemoryBlob temp = RadixAbstract_ConstructPointerBlob(
        (blob.ptr)+offset, length);
    temp.heap = blob.heap;
    return temp;
}

void RadixAbstract_InsertBlob(
    RadixMemoryBlob source,
    RadixMemoryBlob target,
    unsigned long long target_offset)
    { memcpy((target.ptr)+target_offset, source.ptr, source.length); }

unsigned long long RadixAbstract_GetBlobLength(RadixMemoryBlob blob)
    { return blob.length; }

void * RadixAbstract_GetBlobPointer(RadixMemoryBlob blob)
    { return blob.ptr; }

bool RadixAbstract_GetBlobHeapStatus(RadixMemoryBlob blob)
    { return blob.heap; }

void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob)
{
    if (blob->heap && !blob->destroyed) {
        free(blob->ptr);
    }

    blob->destroyed = true;
    blob->length = 0;
}

bool RadixAbstract_BlobIsDestroyed(RadixMemoryBlob blob)
    { return blob.destroyed; }

RadixMemoryBlob RadixAbstract_StrBlob(void *dest)
    { return RadixAbstract_ConstructPointerBlob(dest, strlen(dest)); }

RadixMemoryBlob RadixAbstract_MallocCopy(RadixMemoryBlob src)
{
    RadixMemoryBlob temp = RadixAbstract_MallocBlob(
        RadixAbstract_GetBlobLength(src));

    RadixAbstract_InsertBlob(src, temp, 0);

    return temp;
}

bool RadixAbstract_BlobEmpty(RadixMemoryBlob blob)
    { return (!RadixAbstract_GetBlobLength(blob)) ? true : false; }

bool RadixAbstract_BlobEquals(RadixMemoryBlob b0, RadixMemoryBlob b1)
{
    unsigned long long toCompare = RadixAbstract_GetBlobLength(b0);
    if (toCompare != RadixAbstract_GetBlobLength(b1)) return false;
    if (RadixAbstract_BlobEmpty(b0)) return true;

    const char *ptr0 = (const char *)RadixAbstract_GetBlobPointer(b0);
    const char *ptr1 = (const char *)RadixAbstract_GetBlobPointer(b1);

    while (toCompare--)
        if (*ptr0++ != *ptr1++)
            return false;

    return true;
}

unsigned long long RadixAbstract_BlobContains(
    RadixMemoryBlob haystack,
    RadixMemoryBlob needle,
    unsigned int interval)
{
    RadixMemoryBlob buffer;
    unsigned long long p;
    unsigned long long scanlength = RadixAbstract_GetBlobLength(haystack);

    if (RadixAbstract_GetBlobLength(needle) > scanlength) return 0;

    for (p = 0; p < scanlength; p += MAX(1, interval))
    {
        if (scanlength - p < interval) break;

        buffer = RadixAbstract_SliceBlob(
            haystack, p, RadixAbstract_GetBlobLength(needle));

        if (RadixAbstract_BlobEquals(buffer, needle)) return p + 1;
    }

    return 0;
}