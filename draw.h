#include "v3f.h"
#include "map.h"
#include "object.h"

struct c4f {
  float r, g, b, a;
};

void draw_obj(struct obj *o, struct v3f pos, struct v3f rot);
void draw_map(struct map *m);

void draw_light(int light, struct v3f pos, struct c4f col);
void draw_projectile(struct v3f pos, struct v3f vel);
void draw_explosion(struct v3f pos, float size);
