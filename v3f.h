#ifndef __V3F_H__
#define __V3F_H__


#include <math.h>

struct v3f {
  float x, y, z;
};

static inline float* v3f_to_f3v(struct v3f *v) {
	return (float *) v; // Raw cast
}

static inline struct v3f v3f_create(float x, float y, float z)
{
  struct v3f this;
  this.x = x; this.y = y; this.z = z;
  return this;
}

static inline float* v3f_array(struct v3f *this)
{
  return (float*) this;
}

static inline struct v3f v3f_add(struct v3f this, struct v3f v)
{
  this.x += v.x; this.y += v.y; this.z += v.z;
  return this;
}

static inline struct v3f v3f_sub(struct v3f this, struct v3f v)
{
  this.x -= v.x; this.y -= v.y; this.z -= v.z;
  return this;
}

static inline struct v3f v3f_mulv(struct v3f this, struct v3f v)
{
  this.x *= v.x; this.y *= v.y; this.z *= v.z;
  return this;
}

static inline struct v3f v3f_divv(struct v3f this, struct v3f v)
{
  this.x /= v.x; this.y /= v.y; this.z /= v.z;
  return this;
}

static inline struct v3f v3f_mul(struct v3f this, float s)
{
  this.x *= s; this.y *= s; this.z *= s;
  return this;
}

static inline struct v3f v3f_div(struct v3f this, float s)
{
  this.x /= s; this.y /= s; this.z /= s;
  return this;
}

static inline struct v3f v3f_meg(struct v3f this)
{
  return v3f_mul(this, -1);
}

static inline float v3f_dot_product(struct v3f v1, struct v3f v2)
{
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static inline float v3f_square_magnitude(struct v3f v)
{
  return v3f_dot_product(v, v);
}

static inline float v3f_magnitude(struct v3f v)
{
  return sqrt(v3f_square_magnitude(v));
}

static inline struct v3f v3f_normalize(struct v3f v)
{
  return v3f_div(v, v3f_magnitude(v));
}

static inline struct v3f v3f_cross_product(struct v3f v1, struct v3f v2)
{
  struct v3f res;
  res.x = v1.y * v2.z - v1.z * v2.y;
  res.y = v1.z * v2.x - v1.x * v2.z;
  res.z = v1.x * v2.y - v1.y * v2.x;
  return res;
}

#endif // __V3F_H__