#include <stdio.h>
#include "map.h"

#include "dump.h"

int main(int argc, char **argv) {
	struct map m = { 0 };

	for (int i = 1; i < argc; i ++) {
		map_load(&m, argv[i], 1, 1);

		hexDump("m", &m, sizeof(m), 16);
		hexDump("m->h", m.h, sizeof(*m.h) * m.size_x * m.size_y, 16);

		hexDump("o", &m.o, sizeof(m.o), 16);
		hexDump("o->faces", m.o.faces, sizeof(*m.o.faces) * m.o.nb_faces, 16);
		hexDump("o->vertices", m.o.vertices, sizeof(*m.o.vertices) * m.o.nb_vertices, 16);
		hexDump("o->vertices_texture", m.o.vertices_texture, sizeof(*m.o.vertices_texture) * m.o.nb_vertices_texture, 16);
		hexDump("o->vertices_normal", m.o.vertices_normal, sizeof(*m.o.vertices_normal) * m.o.nb_vertices_normal, 16);
	}
}
