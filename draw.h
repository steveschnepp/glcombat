#include "v3f.h"
#include "map.h"
#include "object.h"

struct draw_options {
    int no_fill : 1;
    int no_wireframe : 1;
    int no_light : 1;
};

struct c4f {
  float r, g, b, a;
};

extern struct draw_options draw_options;

void draw_obj(struct obj *o, struct v3f pos, struct v3f rot);
void draw_map(struct map *m);

void draw_light(int light, struct v3f pos, struct c4f col);
void draw_projectile(struct v3f pos);
