#include <stdint.h>

#include "object.h"
#include "v3f.h"

struct map {
	int size_x, size_y;

	float unit_x, unit_y;
	float *h;
	struct obj o;
};



void map_load(struct map *map, char *filename);
