#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>

#include "object.h"
#include "v3f.h"

struct map {
	int size_x, size_y;

	float unit_x, unit_y;
	float *h;
	struct obj o;
};

void map_load(struct map *map, char *filename, 	int i_step, int j_step);
float map_get_height(struct map *map, float x, float y);

#endif // __MAP_H__
