#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "object.h"

#define MAX_LINE_LEN 64

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
	o->vertices = realloc(o->vertices, o->nb_vertices * sizeof(struct v3f));
	o->vertices[o->nb_vertices-1] = vertices_current; // set the last one

	return o->nb_vertices;
}

int object_add_vertex_texture(struct obj *o, struct t2f vertices_texture_current) {
	// add a new vertex
	o->nb_vertices_texture += 1;
	o->vertices_texture = realloc(o->vertices_texture, o->nb_vertices_texture * sizeof(struct t2f));
	o->vertices_texture[o->nb_vertices_texture-1] = vertices_texture_current;

	return o->nb_vertices_texture;
}

int object_add_vertex_normal(struct obj *o, struct v3f vertices_normal_current) {
	// add a new vertex
	o->nb_vertices_normal += 1;
	o->vertices_normal = realloc(o->vertices_normal, o->nb_vertices_normal * sizeof(struct v3f));
	o->vertices_normal[o->nb_vertices_normal-1] = vertices_normal_current;

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
	ret = sscanf(str1, "%d %d %d", &faces.face[0].vertice, &faces.face[0].texture, &faces.face[0].normal);
	assert(ret == 3);

	object_str_tr(str2, '/', ' ');
	ret = sscanf(str2, "%d %d %d", &faces.face[1].vertice, &faces.face[1].texture, &faces.face[1].normal);
	assert(ret == 3);

	object_str_tr(str3, '/', ' ');
	ret = sscanf(str3, "%d %d %d", &faces.face[2].vertice, &faces.face[2].texture, &faces.face[2].normal);
	assert(ret == 3);

	return faces;
}

// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
int object_add_face(struct obj *o, struct obj_faces faces_current) {
	// add a new vertex
	o->nb_faces += 1;
	o->faces = realloc(o->faces, o->nb_faces * sizeof(struct obj_faces));
	o->faces[o->nb_faces-1] = faces_current;

	return o->nb_faces;
}

void object_read_from_file(struct obj *o, char *filename)
{
	// Remove the indirect arrays
	if (o->faces) free(o->faces);
	if (o->vertices) free(o->vertices);
	if (o->vertices_texture) free(o->vertices_texture);
	if (o->vertices_normal) free(o->vertices_normal);

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
}
