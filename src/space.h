#include "glbl.h"
#include "bool.h"

#ifndef RIVEC_HEADER_FILE
#define RIVEC_HEADER_FILE
    typedef struct R_point2d {
        float a;
        float b;
    } R_point2d;

    typedef struct R_point3d {
        float a;
        float b;
        float c;
    } R_point3d;

    typedef struct R_mesh3d {
        bool heap;
        unsigned short vertices;
        R_point3d *points;
    } R_mesh3d;
#endif