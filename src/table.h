/*
 * @table.h
 * Defines the RadixTable, an "object" of sorts that operates similarly to
 * a hashmap or a Python "dict."
 * 
 * Be careful when using the table. All of its keys, values and indexes are
 * stored in the heap, and you must manually destroy the table.
 * 
 * WARNING: When using any function that returns the key of a table element,
 * DO NOT MUTATE THE KEY. If you change the contents of a key blob without
 * calling RadixTable_ChangeKey, you will accidentally cause the key hash and
 * key contents to lose sync. This WILL break everything! Not that you'd need
 * to modify the key that much anyway.
 */

#include "glbl.h"
#include "primitives.h"

#ifndef FTABL_HEADER_FILE
#define FTABL_HEADER_FILE
    // IMPORTANT: Read this for query flags
    #include "linked_shared.h"

    /* ## private structs ##
     * These are structures used internally to represent a table. You won't
     * have to interface with them yourself - you'll only have to use them as
     * variable types.
     */
	typedef struct RadixTableElement {
	    // This allows for better iterative performance when searching for keys
	    uint64_t keyHash;
	    // This provides a key/value pair similar to Python's dicts
	    RadixMemoryBlob key;
	    RadixMemoryBlob value;
	    // Since this is based on the concept of a linked list...
	    struct RadixTableElement *next_element;
	} RadixTableElement;

	typedef struct RadixTable {
	    // The length of the table, which changes during SetItem and
        // DestroyItem
	    unsigned long long length;
	    // The very first element of the table, which starts the chain.
	    RadixTableElement *first_element;
	    // Whether or not to insert at the end
	    bool insert_at_end;
	} RadixTable;

	typedef struct RadixTableKeyIterator {
	    // Points back to the table, so you always know the source.
	    RadixTable *table;
	    // The current element that the iterator is hovering over.
	    RadixTableElement *element;
	    // The current index the iterator is hovering over, starting from 0.
	    unsigned long long index;
	    // FWD/BKWD
	    RadixTableElement *previous;
	    RadixTableElement *next;
	    // Warning flags
	    bool exhausted;
	} RadixTableKeyIterator;

	typedef struct RadixTableQueryResult {
	    bool found;
	    RadixIndex index;
	    RadixTableElement *previous;
	    RadixTableElement *current;
	    RadixTableElement *next;
	} RadixTableQueryResult;

	typedef struct RadixTableQuery {
	    char query_for;
	    uint64_t keyHash;
	    RadixMemoryBlob *key;
	    RadixMemoryBlob *value;
	    unsigned long long index;
	    unsigned int containsInterval;
	} RadixTableQuery;

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

    /* -- RadixTable_KeyIteratorNext --
     * Move the key iterator to the next key.
     */
    void RadixTable_KeyIteratorNext(RadixTableKeyIterator *ki);

    /* -- RadixTable_KeyIteratorNext --
     * Returns true if the current element in the key iterator matches the
     * given query. False if it doesn't.
     */
    bool RadixTable_KeyIteratorCheckElement(
        RadixTableKeyIterator *ki,
        RadixTableQuery *kq);

    /* -- RadixTable_ConstructQuery --
     * Allows you to construct a query that may be used to find items in a
     * table with either RadixTable_Query or RadixTable_KeyIteratorCheckElement
     * 
     * Queries may be reused for multiple tables.
     * 
     * query_for must be in the format of RadixTableQueryFlags
     * (e.g QUERY_INDEX | QUERY_KEY). You may set the key
     * and value arguments to null pointers when they are not in use.
     * Index may be 0 if not in use, but if QUERY_INDEX is specified, then 0
     * will be the first element in the table.
     * 
     * For more information about containsInterval, see the documentation for
     * RadixAbstract_BlobContains. If you aren't using a *_CONTAINS value for
     * query_for, you don't need to use it.
     */
    RadixTableQuery RadixTable_ConstructQuery(
        char query_for,
        RadixMemoryBlob *key,
        RadixMemoryBlob *value,
        unsigned long long index,
        unsigned int containsInterval);

    /* -- RadixTable_Query --
     * Returns the first result of the given RadixTableQuery in the table.
     * To get every result, use RadixTable_KeyIteratorCheckElement.
     */
    RadixTableQueryResult RadixTable_Query(
        RadixTable *table,
        RadixTableQuery query);

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
     * 
     * Returns null pointer if no item by key is found
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

    /* -- RadixTable_ValueGet --
     * Searches the table by value and returns the first key that matches the
     * specified value. This is much slower than getting by key.
     * 
     * Will return a null pointer if no value is found.
     */
    RadixMemoryBlob * RadixTable_ValueGet(
        RadixTable *table,
        RadixMemoryBlob value);

    /* -- RadixTable_KeyByIndex --
     * Return the key blob for a given index in the table.
     * 
     * Returns a null pointer if no key is found at that index.
     */
    RadixMemoryBlob * RadixTable_KeyByIndex(
        RadixTable *table,
        unsigned long long index);

    /* -- RadixTable_IndexByKey --
     * Return the index of a given key in the table. Returned in the form of a
     * RadixIndex, which means you can check if it is present with
     * returnval.present.
     */
    RadixIndex RadixTable_IndexByKey(
        RadixTable *table,
        RadixMemoryBlob key);

    /* -- RadixTable_Update --
     * Copy each element in the source table to the destination table.
     * Overwrites any repeated keys in the destination.
     */
    void RadixTable_Update(RadixTable *dest, RadixTable *src);

#endif
