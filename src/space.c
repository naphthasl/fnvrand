#include "glbl.h"
#include "space.h"
#include "bool.h"

R_mesh3d R_MakeMeshFromPoints(unsigned short vertices, ...)
{
    va_list ap;
    R_mesh3d mesh;
    mesh.points = malloc(sizeof(R_point3d) * vertices);
    mesh.vertices = vertices;
    mesh.heap = true;

    va_start(ap, vertices);

    unsigned short j; for (j = 0; j < vertices; j++)
        mesh.points[j] = va_arg(ap, R_point3d);

    va_end(ap);

    return mesh;
}

void R_DestroyMesh(R_mesh3d *mesh)
{
    if (!mesh->points || !mesh->heap) return;

    free(mesh->points);
    mesh->points = NULL;
    mesh->heap = false;
}