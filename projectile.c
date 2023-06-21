#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projectile.h"
#include "map.h"

static void zero_memory(void *s, size_t n)
{
    memset(s, 0, n);
}

int new_bullet_idx = 0;
struct projectile bullets[MAX_BULLETS];

struct projectile* projectile_new(struct v3f pos, struct v3f delta)
{
    // append at the end
    struct projectile* p = bullets + new_bullet_idx;
    zero_memory(p, sizeof(struct projectile));
    p->pos = pos;
    p->delta = delta;
    new_bullet_idx++;

    return p;
}

const struct v3f gravity = { 0, 0, -10 }; // g = 10 m/s

void projectile_update_all(uint32_t time_delta_ms, struct map *map)
{
    float time_delta_s = time_delta_ms / 1000.0;
    int do_gc = 0;

    for(int i = 0; i < new_bullet_idx; i ++) {
        struct projectile *p = bullets + i;
        if (p->props.is_explosion) {
            p->delta.x *= 1.5;
            if (p->delta.x > 3) {
                // Remove it by copying the last one here.
                p[i] = p[new_bullet_idx-1];
                new_bullet_idx --;

                // process this one
                i --;
                continue;
            }
            continue;
        }

        // Acceleration
        struct v3f gravity_in_time = v3f_mul(gravity, time_delta_s);
        p->delta = v3f_add(p->delta, gravity_in_time);

        // Movement
        struct v3f pos_delta_in_time = v3f_mul(p->delta, time_delta_s);
        p->pos = v3f_add(p->pos, pos_delta_in_time);

        // Hitting earth
        float height = map_get_height(map, p->pos.x, p->pos.y);
        if(p->pos.z < height) {
            p->props.is_explosion = 1;
             p->delta.x = 0.1;
             p->delta.y = p->delta.z = 0;
        }
    }
}