#include <stdint.h>

#include "v3f.h"

struct projectile {
    struct {
        int is_used : 1;
        int is_explosion : 1;
    } props;

    struct v3f pos;
    struct v3f delta;
};

#define MAX_BULLETS 1024

extern int max_bullet_idx;
extern struct projectile bullets[MAX_BULLETS];

struct projectile* projectile_new(struct v3f pos, struct v3f delta);
void projectile_update_all(uint32_t delta_ms);
