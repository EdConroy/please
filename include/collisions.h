#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

// shape.h

typedef enum {false,true} pbool;

#define cube_set(cube,a,b,c,l,m,n) (cube.x=a,cube.y=b,cube.z=c,\
                                    cube.w=l,cube.h=m,cube.d=n)
#endif