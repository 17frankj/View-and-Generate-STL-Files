#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "myLib.h" 
#include "stl_reader.h"

// Generate random color (RGBA)
vec4 random_color() {
    return (vec4){ (float)(rand()%100)/100.0f,
                   (float)(rand()%100)/100.0f,
                   (float)(rand()%100)/100.0f,
                   1.0f };
}

// Read binary STL file and populate Mesh
Mesh read_stl_binary(const char *filename) {
    Mesh mesh;
    mesh.vertices = NULL;
    mesh.colors = NULL;
    mesh.num_vertices = 0;

    FILE *file = fopen(filename, "rb");
    if(!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return mesh;
    }

    // Skip 80-byte header
    fseek(file, 80, SEEK_SET);

    uint32_t num_triangles;
    fread(&num_triangles, sizeof(uint32_t), 1, file);

    mesh.vertices = malloc(sizeof(vec4) * num_triangles * 3);
    mesh.colors   = malloc(sizeof(vec4) * num_triangles * 3);

    for(uint32_t i = 0; i < num_triangles; i++) {
        float normal[3];
        fread(normal, sizeof(float), 3, file); // normal, can be ignored

        float v[3][3];
        fread(v, sizeof(float), 9, file); // 3 vertices

        for(int j = 0; j < 3; j++) {
            mesh.vertices[mesh.num_vertices] = (vec4){ v[j][0], v[j][1], v[j][2], 1.0f };
            mesh.colors[mesh.num_vertices] = random_color();
            mesh.num_vertices++;
        }

        uint16_t attr_byte_count;
        fread(&attr_byte_count, sizeof(uint16_t), 1, file); // skip
    }

    //printf("%d", mesh.num_vertices);
    fclose(file);
    return mesh;
}

// Optional: normalize mesh to fit in [-1,1] canonical view
void normalize_mesh(Mesh *mesh) {
    float min_x = 1e30f, min_y = 1e30f, min_z = 1e30f;
    float max_x = -1e30f, max_y = -1e30f, max_z = -1e30f;

    for(int i = 0; i < mesh->num_vertices; i++) {
        vec4 v = mesh->vertices[i];
        if(v.x < min_x) min_x = v.x; if(v.x > max_x) max_x = v.x;
        if(v.y < min_y) min_y = v.y; if(v.y > max_y) max_y = v.y;
        if(v.z < min_z) min_z = v.z; if(v.z > max_z) max_z = v.z;
    }

    float center_x = (min_x + max_x) / 2.0f;
    float center_y = (min_y + max_y) / 2.0f;
    float center_z = (min_z + max_z) / 2.0f;

    float scale = fmaxf(max_x - min_x, fmaxf(max_y - min_y, max_z - min_z));
    scale = 2.0f / scale; // fit into [-1,1]

    for(int i = 0; i < mesh->num_vertices; i++) {
        mesh->vertices[i].x = (mesh->vertices[i].x - center_x) * scale;
        mesh->vertices[i].y = (mesh->vertices[i].y - center_y) * scale;
        mesh->vertices[i].z = (mesh->vertices[i].z - center_z) * scale;
    }
}

// Free mesh memory
void free_mesh(Mesh *mesh) {
    if(mesh->vertices) free(mesh->vertices);
    if(mesh->colors) free(mesh->colors);
    mesh->vertices = NULL;
    mesh->colors = NULL;
    mesh->num_vertices = 0;
}

