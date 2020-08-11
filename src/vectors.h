#include "glbl.h"

#ifndef RIVEC_HEADER_FILE
#define RIVEC_HEADER_FILE
    typedef struct R_vec3d {
        float x;
        float y;
        float z;
    } R_vec3d;

    typedef struct R_tris3d {
        R_vec3d a;
        R_vec3d b;
        R_vec3d c;
    } R_tris3d;
#endif