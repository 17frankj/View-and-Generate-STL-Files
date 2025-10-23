/*
 * stl_reader.h
 *
 * 
 *  CS1566 Project 1 Transformations: Created on: Octobor 15, 2025
 *      Author: Joshua Frank
 * 
 * 
 *  ------------ made with Open Ai Chat Gpt
 */
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
