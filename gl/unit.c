#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unit.h"
#include "map.h"

static void zero_memory(void *s, size_t n)
{
    memset(s, 0, n);
}

int new_unit_idx = 0;
struct unit units[MAX_UNITS];

static void unit_update_destination(struct unit *p, struct map *map);

struct unit* unit_new(struct v3f pos)
{
    // append at the end
    struct unit* p = units + new_unit_idx;
    zero_memory(p, sizeof(struct unit));
    p->pos = pos;
    p->destination = pos;
    new_unit_idx++;

    p->color.r = new_unit_idx % 3 == 0;
    p->color.g = new_unit_idx % 3 == 1;
    p->color.b = new_unit_idx % 3 == 2;
    p->color.a = 1;

    return p;
}

static void unit_update_destination(struct unit *p, struct map *map) {
    p->destination.x = rand() % map->size_x - map->size_x / 2;
    p->destination.y = rand() % map->size_y - map->size_y / 2;

    p->destination.x *= map->unit_x * .9;
    p->destination.y *= map->unit_y * .9;
    float height = map_get_height(map, p->pos.x, p->pos.y);
    p->destination.z = height;
    p->speed = (rand() % 10) + 50;
}

void unit_update_all(uint32_t time_delta_ms, struct map *map)
{
    float time_delta_s = time_delta_ms / 1000.0;

    for(int i = 0; i < new_unit_idx; i ++) {
        struct unit *p = units + i;

        // Moving in 2D
        struct v3f dest = p->destination; dest.z = 0;
        struct v3f pos = p->pos; pos.z = 0;
        struct v3f direction = v3f_sub(dest, pos);
        float distance_to_dest2 = v3f_square_magnitude(direction);
        if (distance_to_dest2 < 2.0) {
            unit_update_destination(p, map);
            continue;
        }

        // direction
        struct v3f direction_speed = v3f_mul(v3f_normalize(direction), p->speed);

        // Movement
        struct v3f pos_delta_in_time = v3f_mul(direction_speed, time_delta_s);
        p->pos = v3f_add(p->pos, pos_delta_in_time);

        // Hitting earth
        float height = map_get_height(map, p->pos.x, p->pos.y);
        p->pos.z = height;
    }
}
