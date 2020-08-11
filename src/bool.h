#include "glbl.h"

#ifndef FBOOL_HEADER_FILE
#define FBOOL_HEADER_FILE
    // Just in case it has been defined elsewhere
    #if !(defined(true) || defined(false) || defined(bool))
        #define true 1
        #define false 0
        #define bool char
    #endif

    #define BOOLIFY(a) ((a)?(true):(false))
#endif