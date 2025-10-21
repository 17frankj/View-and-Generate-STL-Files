#ifndef _STL_READER_H_

#define _STL_READER_H_

typedef struct {
    vec4 *vertices;   // array of vertices (vec4)
    vec4 *colors;     // corresponding colors
    int num_vertices; // total number of vertices
} Mesh;

Mesh read_stl_binary(const char *filename);
void normalize_mesh(Mesh *mesh);
void free_mesh(Mesh *mesh);

// Do not put anything after this line

#endif
