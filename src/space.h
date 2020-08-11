#include "glbl.h"

#ifndef RIVEC_HEADER_FILE
#define RIVEC_HEADER_FILE
    typedef struct R_point3d {
        float x;
        float y;
        float z;
    } R_point3d;

    typedef struct R_line3d {
        R_point3d a;
        R_point3d b;
    } R_line3d;

    typedef struct R_tris3d {
        R_point3d a;
        R_point3d b;
        R_point3d c;
    } R_tris3d;

    typedef struct R_square3d {
        R_tris3d a;
        R_tris3d b;
    } R_square3d;
#endif