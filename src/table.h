/*
 * @table.h
 * Defines the RadixTable, an "object" of sorts that operates similarly to
 * a hashmap or a Python "dict."
 * 
 * Be careful when using the table. All of its keys, values and indexes are
 * stored in the heap, and you must manually destroy the table.
 */

#include <stdint.h>
#include "primitives.h"

#ifndef FLIST_HEADER_FILE
#define FLIST_HEADER_FILE
    /* ## private structs ##
     * These are structures used internally to represent a table. You won't
     * have to interface with them yourself - you'll only have to use them as
     * variable types.
     */
    typedef struct RadixTableElement RadixTableElement;
    typedef struct RadixTable RadixTable;
    typedef struct RadixTableKeyIterator RadixTableKeyIterator;

    /* -- RadixTable_HashKey --
     * Hashes a blob so it can be used to optimize key retrieval. You do not
     * have to use this, it is included for internal use.
     */
    uint64_t RadixTable_HashKey(RadixMemoryBlob key);

    /* -- RadixTable_New --
     * Create a new table.
     */
    RadixTable RadixTable_New();

    /* -- RadixTable_Length --
     * Return the amount of elements in a table.
     */
    unsigned long long RadixTable_Length(RadixTable *table);

    /* -- RadixTable_NewKeyIterator --
     * Return a key iterator structure, used to iterate over each key in a
     * table.
     */
    RadixTableKeyIterator RadixTable_NewKeyIterator(RadixTable *table);

    /* -- RadixTable_KeyIteratorGet --
     * Get the current table element stored at the key iterator's current key.
     */
    RadixTableElement * RadixTable_KeyIteratorGet(RadixTableKeyIterator *ki);

    /* -- RadixTable_KeyIteratorNext --
     * Move the key iterator to the next key.
     */
    void RadixTable_KeyIteratorNext(RadixTableKeyIterator *ki);

    /* -- RadixTable_Find --
     * Iterates over the whole table and returns a RadixTableElement pointer
     * that corresponds to the specified key.
     */
    RadixTableElement * RadixTable_Find(
        RadixTable *table,
        RadixMemoryBlob key);

    /* -- RadixTable_In --
     * Check if a key is inside the table. True if it is, False if it isn't.
     */
    bool RadixTable_In(RadixTable *table, RadixMemoryBlob key);

    /* -- RadixTable_GetItem --
     * Return a pointer to the RadixMemoryBlob for the value of the specified
     * key in the table. Mutations to the RadixMemoryBlob, if using the
     * pointer, should be maintained within the table.
     * 
     * DO NOT call RadixAbstract_DestroyBlob on the output of this function, it
     * will completely break the table. I think. Use RadixTable_DestroyItem
     * instead.
     */
    RadixMemoryBlob * RadixTable_GetItem(
        RadixTable *table,
        RadixMemoryBlob key);

    /* -- RadixTable_SetItem --
     * Set an item in the table to the given key and value. You may destroy
     * the blobs you used for the key and value after this - they are copied
     * into a mallocated memory region. It is recommended to destroy the key
     * and value blobs afterwards, actually. (Use RadixAbstract_DestroyBlob)
     */
    void RadixTable_SetItem(
        RadixTable *table,
        RadixMemoryBlob key,
        RadixMemoryBlob value);

    /* -- RadixTable_ChangeKey --
     * "Rename" an index within the table by changing the key to a new key.
     * The key and new_key values here are copied into a mallocated memory
     * region, you are free to call RadixAbstract_DestroyBlob on them after
     * their use (in fact, it is recommended).
     */
    bool RadixTable_ChangeKey(
        RadixTable *table,
        RadixMemoryBlob key,
        RadixMemoryBlob new_key);

    /* -- RadixTable_DestroyItem --
     * Completely remove a key/value pair from the table by key.
     */
    bool RadixTable_DestroyItem(RadixTable *table, RadixMemoryBlob key);

    /* -- RadixTable_DestroyTable --
     * Destroy the entire table. DO NOT forget to call this when you are done
     * using it!
     */
    void RadixTable_DestroyTable(RadixTable *table);

    #include "table.c"
#endif