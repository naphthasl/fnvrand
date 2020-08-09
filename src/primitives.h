/*
 * @primitives.h
 * Covers a set of basic types that Radix introduces in order to, hopefully,
 * make your life easier.
 * 
 * Among these is the RadixMemoryBlob, which itends to make manipulation of
 * data in memory easier, as well as memory management and bounds checking.
 */
#include "bool.h"

#ifndef FPRIM_HEADER_FILE
#define FPRIM_HEADER_FILE
    /* -- RandixMemoryBlob --
     * Structure representing a starting pointer and the length of the
     * whole memory range.
     */
    typedef struct RadixMemoryBlob RadixMemoryBlob;

    /* -- RadixAbstract_ConstructPointerBlob --
     * Takes a given pointer and memory range length and uses it to construct
     * a neat and tidy RadixMemoryBlob. Does not copy the memory. Think of it
     * sort of like how Copy-On-Write works, but without the copying part.
     */
    RadixMemoryBlob RadixAbstract_ConstructPointerBlob(
        void *dest,
        unsigned long long length);

    /* -- RadixAbstract_MallocBlob --
     * Behaves similarly to RadixAbstract_ConstructPointerBlob, creates an
     * empty malloc in the heap of arbritrary length and returns it as a
     * RadixMemoryBlob.
     */
    RadixMemoryBlob RadixAbstract_MallocBlob(unsigned long long length);

    /* -- RadixAbstract_SliceBlob --
     * Allows you to take slices out of a RadixMemoryBlob. Does not copy the
     * memory. If you want to do that, combine it with RadixAbstract_InsertBlob
     * and RadixAbstract_MallocBlob.
     */
    RadixMemoryBlob RadixAbstract_SliceBlob(
        RadixMemoryBlob *blob,
        unsigned long long offset,
        unsigned long long length);

    /* -- RadixAbstract_InsertBlob --
     * Allows you to insert the entirety of a source blob at a given offset
     * in the target blob. Does not grow the blob automatically.
     */
    void RadixAbstract_InsertBlob(
        RadixMemoryBlob *source,
        RadixMemoryBlob *target,
        unsigned long long target_offset);

    /* -- RadixAbstract_GetBlobLength --
     * Returns the length of a blob. Useful for manipulation purposes.
     */
    unsigned long long RadixAbstract_GetBlobLength(RadixMemoryBlob *blob);

    /* -- RadixAbstract_GetBlobPointer --
     * Returns the pointer for the start of the blob. Useful for reading and
     * writing to the blob.
     */
    void * RadixAbstract_GetBlobPointer(RadixMemoryBlob *blob);

    /* -- RadixAbstract_DestroyBlob --
     * Removes the blob from memory. This is neccessary for any malloc'd blobs.
     * Generally best practice to call this regardless. Or use a garbage
     * collector.
     */
    void RadixAbstract_DestroyBlob(RadixMemoryBlob *blob);

    /* -- RadixAbstract_GetBlobHeapStatus --
     * Allows you to check if a blob is stored within the heap or not. This
     * lets you check if you can do things like reallocating the blob.
     */
    bool RadixAbstract_GetBlobHeapStatus(RadixMemoryBlob *blob);

    /* -- RadixAbstract_ResizeMallocBlob --
     * Allows you to modify the size of a blob in the heap. Make sure to check
     * if it's actually in the heap or not before calling this, though.
     * (Use RadixAbstract_GetBlobHeapStatus)
     */
    void RadixAbstract_ResizeMallocBlob(
        RadixMemoryBlob *blob,
        unsigned long long new_length);

    #include "primitives.c"
#endif