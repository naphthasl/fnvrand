#include "glbl.h"
#include "bool.h"

#ifndef RIVEC_HEADER_FILE
#define RIVEC_HEADER_FILE
    typedef struct R_point2d {
        bool active;
        float a;
        float b;
    } R_point2d;

    typedef struct R_point3d {
        bool active;
        float a;
        float b;
        float c;
    } R_point3d;

    typedef struct R_mesh3d {
        bool heap;
        unsigned short vertices;
        R_point3d *points;
    } R_mesh3d;

    /* Beware! This uses malloc. Maybe do something like this...
     * 
     * R_mesh3d myMesh = {
     *     false, 4, {
     *         {true, 0, 0 , 0},
     *         {true, 1, 0 , 0},
     *         {true, 1, -1, 0},
     *         {true, 0, -1, 0}
     *     }
     * };
     * 
     * Except the layout of this may change at any time since you're directly
     * interfacing with the structure, which kinda sucks.
     * 
     * This does provide a standard way to create a mesh in
     * the event that the layout changes, so maybe you'd prefer to do it this
     * way. As long as your meshes aren't too complex, it's probably not an
     * issue.
     */
    R_mesh3d R_MakeMeshFromPoints(unsigned short vertices, ...);

    // Returns a copy of a mesh, performed using malloc and memcpy.
    R_mesh3d R_CopyMesh(R_mesh3d *mesh);

    // Destroy a mesh, attempting to free memory too if possible.
    void R_DestroyMesh(R_mesh3d *mesh);

    // Move a mesh to the heap. You will never really need to call this, it is
    // done automatically when needed.
    void R_MoveMeshPointsToHeap(R_mesh3d *mesh);
    
    // Change how many points a mesh has. Useful for fast filling.
    void R_ChangeMeshPointsSize(R_mesh3d *mesh, unsigned short size);

    // Set a single point in the mesh. Will grow the mesh if i (index) is above
    // or equal to the length of the mesh points array.
    void R_SetMeshPoint(R_mesh3d *mesh, unsigned short i, R_point3d point);

    // Remove a point in the mesh.
    void R_RemoveMeshPoint(R_mesh3d *mesh, unsigned short i);

    // Return a given point in the mesh.
    R_point3d R_GetMeshPoint(R_mesh3d *mesh, unsigned short i);

    // Get the amount of vertices/length of the mesh
    unsigned short R_GetMeshVertices(R_mesh3d *mesh);
#endif