#include "glbl.h"
#include "primitives.h"
#include "bool.h"

// IMPORTANT: Read this for query flags
#include "linked_shared.h"

#ifndef FLIST_HEADER_FILE
#define FLIST_HEADER_FILE
    /* ## private structs ##
     * These are structures used internally to represent a list. You won't
     * have to interface with them yourself - you'll only have to use them as
     * variable types.
     */

    // Result from querying the list
    typedef struct RadixListQueryResult {
        // Was the query successful?
        bool found;
        // The index that the query succeeded on
        RadixIndex index;
        // The previous element, successful element and next element
        RadixListElement *previous;
        RadixListElement *current;
        RadixListElement *next;
    } RadixListQueryResult;

    // A query input - what to search for
    typedef struct RadixListQuery {
        // See linked_shared -> RadixIteratorQueryFlags for possible query_for
        // values.
        char query_for;
        // Value to search for. Can be NULL pointer.
        RadixMemoryBlob *value;
        // Index to query for. Can be 0.
        unsigned long long index;
        // Interval to use for QUERY_VALUE_CONTAINS
        unsigned int containsInterval;
    } RadixListQuery;

    // Sides to append/concatenate to the list
    enum RadixListSides {
        LIST_SIDE_LEFT = false,
        LIST_SIDE_RIGHT = true
    };

    // Create a new list
    RadixList RadixList_New();

    // Check if a list iterator's current element matches the given query
    bool RadixList_IteratorCheckElement(
        RadixListIterator *ki,
        RadixListQuery *kq);

    // Construct a query for the list - see the RadixListQuery struct
    RadixListQuery RadixList_ConstructQuery(
        char query_for,
        RadixMemoryBlob *value,
        unsigned long long index,
        unsigned int containsInterval);

    // Carry out a query on the list and return the first valid result.
    RadixListQueryResult RadixList_Query(
        RadixList *list,
        RadixListQuery query);

    // Find an element by index in the list.
    RadixListElement * RadixList_FindIndex(
        RadixList *list,
        unsigned long long index);

    // Find an element by value in the list.
    RadixListElement * RadixList_FindValue(
        RadixList *list,
        RadixMemoryBlob value);

    // Check if an index exists in the list
    bool RadixList_IndexIn(RadixList *list, unsigned long long index);

    // Check if a value exists in the list
    bool RadixList_ValueIn(RadixList *list, RadixMemoryBlob value);

    // Turn a list index into a value - you can modify this blob too!
    RadixMemoryBlob * RadixList_GetValue(
        RadixList *list,
        unsigned long long index);

    // Get an index by value, useful for Python-like remove()s.
    RadixIndex RadixList_GetIndex(RadixList *list, RadixMemoryBlob value);

    // Set an existing index to the given value, returns false if the index
    // doesn't exist.
    bool RadixList_SetIndex(
        RadixList *list,
        unsigned long long index,
        RadixMemoryBlob value);

    // Insert a new element that will appear at the given position.
    // (E.g 0 places the element right at the start.)
    // Returns the position.
    unsigned long long RadixList_InsertNew(
        RadixList *list,
        unsigned long long new_position);

    // Insert a new element at the right or left side of the list, specified
    // by the RadixListSides enum. Returns the new position.
    unsigned long long RadixList_Append(RadixList *list, char side);

    // Same as RadixList_Append, but it appends an actual value rather than a
    // blank new element.
    unsigned long long RadixList_AppendValue(
        RadixList *list,
        char side,
        RadixMemoryBlob value);

    // Swap the values of two indexes in the list, useful for sorting.
    void RadixList_SwapIndexValues(
        RadixList *list,
        unsigned long long p0,
        unsigned long long p1);

    // Remove a given index from the list, deallocating the memory used.
    bool RadixList_DestroyIndex(RadixList *list, unsigned long long index);

    // Destroy the entire list, completely dealloacting all the memory it uses.
    void RadixList_DestroyList(RadixList *list);

    // Concatenate two lists at the given side, specified by the
    // RadixListSides enum.
    void RadixTable_Concatenate(RadixList *dest, RadixList *src, char side);
#endif
