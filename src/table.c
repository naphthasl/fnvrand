typedef struct RadixTableElement {
    RadixMemoryBlob key;
    RadixMemoryBlob value;
} RadixTableElement;

typedef struct RadixTable {
    unsigned int length;
    RadixTableElement *elements;
} RadixTable;