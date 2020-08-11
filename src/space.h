#include "glbl.h"

#ifndef RIVEC_HEADER_FILE
#define RIVEC_HEADER_FILE
    typedef struct R_point3d {
        float x;
        float y;
        float z;
    } R_point3d;

    typedef struct R_tris3d {
        R_point3d a;
        R_point3d b;
        R_point3d c;
    } R_tris3d;
#endif