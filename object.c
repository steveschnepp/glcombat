#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "object.h"

#define MAX_LINE_LEN 64

static int nb_objects = 0;
static struct obj *objects = 0;

#define QUANTIZE (4 * 1024 * 1024)
static int quantize(int i) {
	i /= QUANTIZE;
	i ++;
	i *= QUANTIZE;
	return i;
}

static void *xrealloc(void *p, size_t s) {
	s = quantize(s);
	return realloc(p, s);
}

static struct v3f object_parse_v3f(char *line) {
	struct v3f v = {0};
	int ret = sscanf(line, "%f %f %f", &v.x, &v.y, &v.z);
	assert(ret == 3);
	return v;
}

static struct t2f object_parse_t2f(char *line) {
	struct t2f t = {0};
	int ret =  sscanf(line, "%f %f", &t.u, &t.v);
	assert(ret == 2);
	return t;
}

// o [object name]
void object_set_name(struct obj *o, char *name) {
	int ret = sscanf(name, "%32s", o->name);
	assert(ret == 1);
}

// v 0.123 0.234 0.345 1.0
int object_add_vertex(struct obj *o, struct v3f vertices_current) {
	// add a new vertex
	o->nb_vertices += 1;
	o->vertices = xrealloc(o->vertices, o->nb_vertices * sizeof(struct v3f));
	o->vertices[o->nb_vertices-1] = vertices_current; // set the last one

	return o->nb_vertices;
}

int object_add_vertex_texture(struct obj *o, struct t2f vertices_texture_current) {
	// add a new vertex
	o->nb_vertices_texture += 1;
	o->vertices_texture = xrealloc(o->vertices_texture, o->nb_vertices_texture * sizeof(struct t2f));
	o->vertices_texture[o->nb_vertices_texture-1] = vertices_texture_current;

	return o->nb_vertices_texture;
}

int object_add_vertex_normal(struct obj *o, struct v3f vertices_normal_current) {
	// add a new vertex
	o->nb_vertices_normal += 1;
	o->vertices_normal = xrealloc(o->vertices_normal, o->nb_vertices_normal * sizeof(struct v3f));
	o->vertices_normal[o->nb_vertices_normal-1] = v3f_normalize(vertices_normal_current);

	return o->nb_vertices_normal;
}

static void object_str_tr(char* s, char from, char to) {
	for(; *s != '\0'; s++) {
		if (*s == from) *s = to;
	}
}

static struct obj_faces object_parse_face(char *line) {
	struct obj_faces faces = {0};
	char str1[64], str2[64], str3[64];
	int ret = sscanf(line, "%s %s %s", str1, str2, str3);
	assert(ret == 3);

	object_str_tr(str1, '/', ' ');
	ret = sscanf(str1, "%u %u %u", &faces.face[0].vertice, &faces.face[0].texture, &faces.face[0].normal);
	assert(ret == 3);

	object_str_tr(str2, '/', ' ');
	ret = sscanf(str2, "%u %u %u", &faces.face[1].vertice, &faces.face[1].texture, &faces.face[1].normal);
	assert(ret == 3);

	object_str_tr(str3, '/', ' ');
	ret = sscanf(str3, "%u %u %u", &faces.face[2].vertice, &faces.face[2].texture, &faces.face[2].normal);
	assert(ret == 3);

	return faces;
}

// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
int object_add_face(struct obj *o, struct obj_faces faces_current) {
	// add a new vertex
	o->nb_faces += 1;
	o->faces = xrealloc(o->faces, o->nb_faces * sizeof(struct obj_faces));
	o->faces[o->nb_faces-1] = faces_current;

	return o->nb_faces;
}

struct obj *object_get_by_name(char *name) {
	for (int i = 0; i < nb_objects; i ++) {
		struct obj *o = objects + i;

		if (strcmp(name, o->name) == 0) {
			// Found, shortcut evaluation
			return o;
		}
	}

	// Not found
	return NULL;
}

struct obj *object_new_from_file(char *filename)
{
	nb_objects++;
	objects = xrealloc(objects, nb_objects * sizeof(struct obj));

	struct obj *o = objects + (nb_objects - 1);
	memset(o, 0, sizeof(*o));

	FILE *f = fopen(filename, "r");

	while (!feof(f)) {
		char line[MAX_LINE_LEN] = {0};
		char* ret = fgets(line, sizeof(line), f);
		if (ret == NULL) break;

		switch (line[0]) {
			case 'o':
				object_set_name(o, line + 2);
				break;
			case 'v':
				switch(line[1]) {
				case ' ':
					object_add_vertex(o, object_parse_v3f(line + 2));
					break;
				case 't':
					object_add_vertex_texture(o, object_parse_t2f(line + 3));
					break;
				case 'n':
					object_add_vertex_normal(o, object_parse_v3f(line + 3));
					break;
				}
				break;
			case 'f':
				object_add_face(o, object_parse_face(line + 2));
				break;
			case '\n':
			case '#':
				// Comment. Ignore
				break;
			default:
				fprintf(stderr, "line not supported: %s\n", line);
				break;
		}

	}

	fclose(f);

	object_compile_arrays(o);

	return o;
}

int object_compile_arrays(struct obj *o) {
	o->vertices_array = calloc(o->nb_faces * 3, sizeof(struct v3f));
	o->vertices_normal_array = calloc(o->nb_faces * 3, sizeof(struct v3f));

	for (int i = 0; i < o->nb_faces; i++) {
		struct obj_faces faces = o->faces[i];
		for (int j = 0; j < 3; j++) {
			int compiled_idx = i*3+j;
			struct obj_face face = faces.face[j];

			unsigned int vertice_normal_idx = face.normal;
			if (vertice_normal_idx) {
				vertice_normal_idx --; // off by one in the obj file
				assert(vertice_normal_idx < o->nb_vertices_normal);
				o->vertices_normal_array[compiled_idx] = o->vertices_normal[vertice_normal_idx];
			}

			unsigned int vertice_idx = face.vertice;
			if (vertice_idx) {
				vertice_idx --; // off by one in the obj file
				assert(vertice_idx < o->nb_vertices);
				o->vertices_array[compiled_idx] = o->vertices[vertice_idx];
			}
		}
	}

	return o->nb_faces;
}
