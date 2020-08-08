#include <stdint.h>

#ifndef FLIST_HEADER_FILE
#define FLIST_HEADER_FILE
    typedef struct {
        void *key;
        void *value;
    } RadixListElement;

    typedef struct {
        unsigned int length;
        RadixListElement *elements;
    } RadixList;

    #include "list.c"
#endif