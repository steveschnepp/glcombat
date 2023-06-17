#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <stdint.h>

#include "v3f.h"

struct obj_face {
	uint32_t vertice;
	uint32_t texture;
	uint32_t normal;
	uint32_t dummy; // 4 fields
};

struct obj_faces {
	struct obj_face face[3];
};

struct t2f {
	float u, v;
};

struct obj {
	char name[32];

	int nb_vertices;
	int nb_vertices_texture;
	int nb_vertices_normal;
	int nb_faces;

	struct v3f* vertices;
	struct t2f* vertices_texture;
	struct v3f* vertices_normal;
	struct obj_faces *faces;
};

void object_set_name(struct obj *o, char *name);
int object_add_vertex(struct obj *o, struct v3f vertex);
int object_add_vertex_texture(struct obj *o, struct t2f texture);
int object_add_vertex_normal(struct obj *o, struct v3f normal);
int object_add_face(struct obj *o, struct obj_faces face);

void object_read_from_file(struct obj *o, char *filename);

#endif // __OBJECT_H__