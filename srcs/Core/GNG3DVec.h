#ifndef GNG3DVEC_H
#define GNG3DVEC_H

#include <math.h>
#include <GL/gl.h>

#define GNG3DVEC_FLOAT GLfloat

struct Vec3
{
    GNG3DVEC_FLOAT x, y, z;
};

extern void Vec3_init(
        struct Vec3* v,
        GNG3DVEC_FLOAT x,
        GNG3DVEC_FLOAT y,
        GNG3DVEC_FLOAT z);

float Vec3_len(struct Vec3* v);
void  Vec3_add(struct Vec3* r, struct Vec3* v1, struct Vec3* v2);
void  Vec3_sub(struct Vec3* r, struct Vec3* v1, struct Vec3* v2);
void  Vec3_scale(struct Vec3* r, GNG3DVEC_FLOAT s);

#endif
