#include "collisions.h"

int cube_cube_intersection(
	Cube a, 
	Cube b)
{
	if ((a.x > b.x + b.w) || (b.x > a.x + a.w) ||
        (a.y > b.y + b.h) || (b.y > a.y + a.h) ||
        (a.z > b.z + b.d) || (b.z > a.z + a.d))
	{
		return false;
	}
	return true;
}