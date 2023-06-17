#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "map.h"

#define MAP_X 128
#define MAP_Y 128

static int middle(int min, int i, int max) {
	if (i < min) return min;
	if (i > max) return max;
	return i;
}

static inline float* map_get_cell(struct map *m, int x, int y) {
	x = middle(0, x, m->size_x);
	y = middle(0, y, m->size_y);
	return m->h + (x * m->size_y) + y;
}

static int map_get_vertice(int i, int j, int size_y) {
	int idx = i * size_y + j;
	return idx + 1; // obj facets begins with 1
}

void map_load(struct map *map, char *filename, 	int i_step, int j_step)
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
	for (int i = i_step; i < MAP.size_x; i+=i_step) {
		for (int j = j_step; j < MAP.size_y; j+=j_step) {
			int a =  map_get_vertice(i, j, MAP.size_y);
			int b =  map_get_vertice(i-i_step, j, MAP.size_y);
			int c =  map_get_vertice(i, j-j_step, MAP.size_y);
			int d =  map_get_vertice(i-i_step, j-j_step, MAP.size_y);

			struct v3f va = MAP.o.vertices[a-1];
			struct v3f vb = MAP.o.vertices[b-1];
			struct v3f vc = MAP.o.vertices[c-1];
			struct v3f vd = MAP.o.vertices[d-1];

			// Compute the middle vertex
			struct v3f ve = va;
			ve = v3f_add(ve, vb);
			ve = v3f_add(ve, vc);
			ve = v3f_add(ve, vd);
			ve = v3f_div(ve, 4);
			int e = object_add_vertex(&MAP.o, ve);

			struct v3f ea = v3f_sub(va, ve);
			struct v3f eb = v3f_sub(vb, ve);
			struct v3f ec = v3f_sub(vc, ve);
			struct v3f ed = v3f_sub(vd, ve);

			struct v3f bea = v3f_cross_product(ea, eb);
			struct v3f aec = v3f_cross_product(ec, ea);
			struct v3f ced = v3f_cross_product(ed, ec);
			struct v3f deb = v3f_cross_product(eb, ed);

			// Autocompute normals
			int bea_idx = object_add_vertex_normal(&MAP.o, bea);
			int aec_idx = object_add_vertex_normal(&MAP.o, aec);
			int ced_idx = object_add_vertex_normal(&MAP.o, ced);
			int deb_idx = object_add_vertex_normal(&MAP.o, deb);

			{
				struct obj_faces faces = {0};
				faces.face[0].vertice = b;
				faces.face[1].vertice = e;
				faces.face[2].vertice = a;

				faces.face[0].normal = bea_idx;
				faces.face[1].normal = bea_idx;
				faces.face[2].normal = bea_idx;

				object_add_face(&MAP.o, faces);
			}

			{
				struct obj_faces faces = {0};
				faces.face[0].vertice = a;
				faces.face[1].vertice = e;
				faces.face[2].vertice = c;

				faces.face[0].normal = aec_idx;
				faces.face[1].normal = aec_idx;
				faces.face[2].normal = aec_idx;

				object_add_face(&MAP.o, faces);
			}

			{
				struct obj_faces faces = {0};
				faces.face[0].vertice = c;
				faces.face[1].vertice = e;
				faces.face[2].vertice = d;

				faces.face[0].normal = ced_idx;
				faces.face[1].normal = ced_idx;
				faces.face[2].normal = ced_idx;

				object_add_face(&MAP.o, faces);
			}

			{
				struct obj_faces faces = {0};
				faces.face[0].vertice = d;
				faces.face[1].vertice = e;
				faces.face[2].vertice = b;

				faces.face[0].normal = deb_idx;
				faces.face[1].normal = deb_idx;
				faces.face[2].normal = deb_idx;

				object_add_face(&MAP.o, faces);
			}
		}
	}

	*map = MAP;
}

static float max(float a, float b)
{
	return a > b ? a : b;
}

float map_get_height(struct map *m, float x, float y)
{
	// get max for the whole cell
	float i_as_f = x / m->unit_x + m->size_x / 2;
	float j_as_f = y / m->unit_y + m->size_y / 2;

	int i = i_as_f;
	int j = j_as_f;

	i_as_f -= i;
	j_as_f -= j;

	assert(i < m->size_x);
	assert(j < m->size_y);

	float z;
	z = *map_get_cell(m, i, j);
	z = max(z, *map_get_cell(m, i+1, j));
	z = max(z, *map_get_cell(m, i, j+1));
	z = max(z, *map_get_cell(m, i+1, j+1));

	return z;
}
