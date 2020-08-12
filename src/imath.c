#include "glbl.h"
#include "imath.h"
#include "macros.h"

unsigned long long R_llupower(unsigned long long x, unsigned long long y)
    { y = MAX(1, y); while ((y--) - 1) x *= x; return x; }