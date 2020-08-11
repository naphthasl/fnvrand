/*
 * @hex.h
 * Allows for the conversion of memory ranges or blobs to their hexidecimal
 * representations.
 */

#include "glbl.h"
#include "primitives.h"

#ifndef FUTIL_HEADER_FILE
#define FUTIL_HEADER_FILE
    /* These functions are for internal use only. */
    void u_memtohex(void *dest, const void *src, register uint64_t numSrcBytes);
    void u_printmemhex(const void *src, register uint64_t numSrcBytes);

    /* These functions are intended for public use. */

    /* -- RadixAbstract_BlobToHex --
     * Returns a new blob containing the hexidecimal representation of the
     * input data.
     * 
     * This function mallocates a blob that is twice the size of the original.
     * 
     * Do NOT forget to destroy it with RadixAbstract_DestroyBlob when you are
     * done.
     */
    RadixMemoryBlob RadixAbstract_BlobToHex(RadixMemoryBlob data);

#endif
