#include <stdint.h>
#include "primitives.h"
#include "list.h"

typedef struct RadixListElement {
    RadixMemoryBlob value;
    struct RadixListElement *next_element;
} RadixListElement;

typedef struct RadixList {
    unsigned long long length;
    RadixListElement *first_element;
} RadixList;

typedef struct RadixTableKeyIterator {
    RadixList *list;
    RadixListElement *element;
    unsigned long long index;
} RadixTableKeyIterator;

