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

R_mesh3d R_CopyMesh(R_mesh3d *mesh)
{
    R_mesh3d temp;
    unsigned int size = sizeof(R_point3d) * mesh->vertices;

    temp.points = malloc(size);
    memcpy(temp.points, mesh->points, size);
    temp.vertices = mesh->vertices;
    temp.heap = true;

    return temp;
}

void R_DestroyMesh(R_mesh3d *mesh)
{
    if (mesh->points && mesh->heap) free(mesh->points);

    mesh->points = NULL;
    mesh->heap = false;
    mesh->vertices = 0;
}

void R_MoveMeshPointsToHeap(R_mesh3d *mesh)
{
    if (mesh->heap) return;

    unsigned int size = sizeof(R_point3d) * mesh->vertices;

    R_point3d *temp = malloc(size);
    memcpy(temp, mesh->points, size);
    mesh->points = temp;
    mesh->heap = true;
}

void R_ChangeMeshPointsSize(R_mesh3d *mesh, unsigned short size)
{
    if (size == mesh->vertices) return;

    R_MoveMeshPointsToHeap(mesh);
    mesh->points = realloc(mesh->points, sizeof(R_point3d) * size);
    mesh->vertices = size;
}

void R_SetMeshPoint(R_mesh3d *mesh, unsigned short i, R_point3d point)
{
    if (i >= mesh->vertices) R_ChangeMeshPointsSize(mesh, i + 1);

    mesh->points[i] = point;
}

void R_RemoveMeshPoint(R_mesh3d *mesh, unsigned short i)
{
    if (i >= mesh->vertices || mesh->vertices < 1) return;

    R_MoveMeshPointsToHeap(mesh);

    R_point3d temp;
    temp = mesh->points[mesh->vertices - 1];
    mesh->points[i] = temp;

    mesh->vertices--;
    mesh->points = realloc(mesh->points, sizeof(R_point3d) * mesh->vertices);
}

R_point3d R_GetMeshPoint(R_mesh3d *mesh, unsigned short i)
{
    R_point3d blank = {false, 0, 0, 0};

    if (i >= mesh->vertices || mesh->vertices < 1) return blank;

    return mesh->points[i];
}

unsigned short R_GetMeshVertices(R_mesh3d *mesh)
    { return mesh->vertices; }