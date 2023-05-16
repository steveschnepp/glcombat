#include <stdio.h>
#include <stdlib.h>

#include "map.h"

#define MAP_X 128
#define MAP_Y 128

static inline float* map_get_cell(struct map *m, int x, int y) {
	return m->h + (x * m->size_y) + y;
}

static int map_get_vertice(int i, int j, int size_y) {
	int idx = i * size_y + j;
	return idx + 1; // obj facets begins with 1
}

void map_load(struct map *map, char *filename)
{
	FILE *f = fopen(filename, "r");
	struct map MAP = {0};

	fscanf(f, "%d", &MAP.size_x);
	fscanf(f, "%d", &MAP.size_y);

	fscanf(f, "%f", &MAP.unit_x);
	fscanf(f, "%f", &MAP.unit_y);

	MAP.h = malloc(MAP.size_x * MAP.size_y * sizeof(float));

	for (int x = 0; x < MAP.size_x; x ++) for (int y = 0; y < MAP.size_y; y ++) {
		fscanf(f, "%f", map_get_cell(&MAP, x, y));
	}

	fclose(f);

	// Precompute the object
	object_set_name(&MAP.o, filename);

	// Vertexes
	for (int i = 0; i < MAP.size_x; i++) {
		for (int j = 0; j < MAP.size_y; j++) {
			float x = MAP.unit_x * (i - MAP.size_x / 2);
			float y = MAP.unit_y * (j - MAP.size_y / 2);
			float z = *map_get_cell(&MAP, i, j);

			struct v3f vertices_current = { x, y, z, };

			object_add_vertex(&MAP.o, vertices_current);
		}
	}

	// Faces
	for (int i = 1; i < MAP.size_x; i++) {
		for (int j = 1; j < MAP.size_y; j++) {
			int a =  map_get_vertice(i, j, MAP.size_y);
			int b =  map_get_vertice(i-1, j, MAP.size_y);
			int c =  map_get_vertice(i, j-1, MAP.size_y);
			int d =  map_get_vertice(i-1, j-1, MAP.size_y);

			struct v3f va = MAP.o.vertices[a-1];
			struct v3f vb = MAP.o.vertices[b-1];
			struct v3f vc = MAP.o.vertices[c-1];
			struct v3f vd = MAP.o.vertices[d-1];

			// Autocompute normals
			int n1, n2;
			{
				struct v3f ab = v3f_sub(vb, va);
				struct v3f ac = v3f_sub(vc, va);
				struct v3f normal = v3f_cross_product(ab, ac);

				n1 = object_add_vertex_normal(&MAP.o, normal);
			}
			{
				struct v3f cd = v3f_sub(vd, vc);
				struct v3f cb = v3f_sub(vb, vc);
				struct v3f normal = v3f_cross_product(cb, cd);
				n2 = object_add_vertex_normal(&MAP.o, normal);
			}

			{
				struct obj_faces faces = {0};

				faces.face[0].vertice = a;
				faces.face[1].vertice = b;
				faces.face[2].vertice = c;

				faces.face[0].normal = n1;
				faces.face[1].normal = n1;
				faces.face[2].normal = n1;

				object_add_face(&MAP.o, faces);
			}

			{
				struct obj_faces faces = {0};

				faces.face[0].vertice = c;
				faces.face[1].vertice = d;
				faces.face[2].vertice = b;

				faces.face[0].normal = n2;
				faces.face[1].normal = n2;
				faces.face[2].normal = n2;

				object_add_face(&MAP.o, faces);
			}
		}
	}

	*map = MAP;
}
