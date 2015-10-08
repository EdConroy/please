#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

typedef struct Cube_S
{
    float x,y,z;
    float w,h,d;
}Cube;

#define cube_set(cube,a,b,c,l,m,n) (cube.x=a,cube.y=b,cube.z=c,\
                                    cube.w=l,cube.h=m,cube.d=n)
/**
 * @brief Detects intersections between two cubes
 * @param first cube
 * @param second cube
 */
int cube_cube_intersection(
	Cube a, 
	Cube b);

#endif