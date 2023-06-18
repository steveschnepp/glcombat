#include <stdio.h>
#include "object.h"

#include "dump.h"

int main(int argc, char **argv) {
	struct obj o = { 0 };

	for (int i = 1; i < argc; i ++) {
		struct obj *o = object_new_from_file(argv[i]);

		hexDump("o", o, sizeof(*o), 16);
		hexDump("o->faces", o->faces, sizeof(*o->faces) * o->nb_faces, 16);
		hexDump("o->vertices", o->vertices, sizeof(*o->vertices) * o->nb_vertices, 16);
		hexDump("o->vertices_texture", o->vertices_texture, sizeof(*o->vertices_texture) * o->nb_vertices_texture, 16);
		hexDump("o->vertices_normal", o->vertices_normal, sizeof(*o->vertices_normal) * o->nb_vertices_normal, 16);
		hexDump("o->vertices_array", o->vertices_array, sizeof(*o->vertices_array) * o->nb_faces * 3, 16);
		hexDump("o->vertices_normal_array", o->vertices_normal_array, sizeof(*o->vertices_normal_array) * o->nb_faces * 3, 16);
	}
}