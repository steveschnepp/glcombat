#ifndef __UNIT_H__
#define __UNIT_H__

#include <stdint.h>

#include "v3f.h"
#include "draw.h"

struct unit {
    struct v3f pos;
    struct v3f rot;
    struct v3f destination;
    float speed;

    struct c4f color;
};

#define MAX_UNITS (1<<16)

extern int new_unit_idx;
extern struct unit units[MAX_UNITS];

struct map;

struct unit* unit_new(struct v3f pos);
void unit_update_all(uint32_t time_delta_ms, struct map *map);

#endif // __UNIT_H__