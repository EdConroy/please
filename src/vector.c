#include "vector.h"


Vec2D vec2d(float x,float y)
{
    Vec2D v = {x,y};
    return v;
}

Vec3D vec3d(float x,float y, float z)
{
    Vec3D v = {x,y,z};
    return v;
}

Vec3D vec_scale(Vec3D vec, float num)
{
	vec.x *= num;
	vec.y *= num;
	vec.z *= num;

	return vec;
}

Vec4D vec4d(float x,float y, float z, float w)
{
    Vec4D v = {x,y,z,w};
    return v;
}


/*eol@eof*/
