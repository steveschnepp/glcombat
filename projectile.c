#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projectile.h"

static void zero_memory(void *s, size_t n)
{
    memset(s, 0, n);
}

int max_bullet_idx;
struct projectile bullets[MAX_BULLETS];

struct projectile* projectile_new(struct v3f pos, struct v3f delta)
{
    for(int i = 0; i < MAX_BULLETS; i ++) {
        struct projectile *p = bullets + i;
        if (! p->props.in_use) {
            p->props.in_use = 1;
            p->pos = pos;
            p->delta = delta;

            if (max_bullet_idx < i) max_bullet_idx = i;

            return p;
        }
    }

    return NULL;
}

void projectile_gc()
{
    max_bullet_idx = 0;
    for(int i = 0; i < MAX_BULLETS; i ++) {
        struct projectile *p = bullets + i;
        if (p->props.in_use) {
            if (max_bullet_idx < i) max_bullet_idx = i;
        }
    }
}

const struct v3f gravity = { 0, 0, -10 }; // g = 10 m/s

#define MAP_FLOOR -100

void projectile_update_all(uint32_t time_delta_ms)
{
    float time_delta_s = time_delta_ms / 1000.0;
    int do_gc = 0;

    for(int i = 0; i < MAX_BULLETS; i ++) {
        struct projectile *p = bullets + i;
        if (! p->props.in_use) {
            continue;
        }

        // Acceleration
        struct v3f gravity_in_time = v3f_mul(gravity, time_delta_s);
        p->delta = v3f_add(p->delta, gravity_in_time);

        // Movement
        struct v3f pos_delta_in_time = v3f_mul(p->delta, time_delta_s);
        p->pos = v3f_add(p->pos, pos_delta_in_time);

        // Hitting earth
        if(p->pos.z < MAP_FLOOR) {
            p->props.in_use = 0;
            do_gc = 1;
        }
    }

    if (do_gc) projectile_gc();
}