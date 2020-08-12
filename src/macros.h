#include "glbl.h"

#ifndef FMACR_HEADER_FILE
#define FMACR_HEADER_FILE
    #if _WIN32 || _WIN64
    #if _WIN64
    #define ENVIRONMENT64
    #else
    #define ENVIRONMENT32
    #endif
    #endif

    #if __GNUC__
    #if __x86_64__ || __ppc64__
    #define ENVIRONMENT64
    #else
    #define ENVIRONMENT32
    #endif
    #endif

    #define MIN(a,b) \
    ({ typeof (a) _a = (a); \
        typeof (b) _b = (b); \
        _a < _b ? _a : _b; })
    #define MAX(a,b) \
    ({ typeof (a) _a = (a); \
        typeof (b) _b = (b); \
        _a > _b ? _a : _b; })

    #define forever for (;;)
#endif