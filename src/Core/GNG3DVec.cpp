#include "GNG3DVec.h"

void Vec3_init(
        struct Vec3* v,
        GNG3DVEC_FLOAT x,
        GNG3DVEC_FLOAT y,
        GNG3DVEC_FLOAT z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

float Vec3_len(struct Vec3* v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void Vec3_add(struct Vec3* r, struct Vec3* v1, struct Vec3* v2)
{
    r->x = v1->x + v2->x;
    r->y = v1->y + v2->y;
    r->z = v1->z + v2->z;
}

void Vec3_sub(struct Vec3* r, struct Vec3* v1, struct Vec3* v2)
{
    r->x = v1->x - v2->x;
    r->y = v1->y - v2->y;
    r->z = v1->z - v2->z;
}

void Vec3_scale(struct Vec3* r, GNG3DVEC_FLOAT s)
{
    r->x = r->x * s;
    r->y = r->y * s;
    r->z = r->z * s;
}
