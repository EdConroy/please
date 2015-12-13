#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "entity.h" // includes body.h, reduces errors

typedef enum
{
	MTYPE_NONE,	// movetype for teh floor
	MTYPE_PLAYER,
	MTYPE_ENT,	// movetype for all living entities, use gravity when jumped and/or in air
	MTYPE_PROJ

} movetype;

void physics_add_body(Body* body);
void physics_remove_body(Body* body);
void physics_collision(Body* body);
void physics_clear_bodies();

/*

new physics structure

1. create body
2. add body to body list
3. update bodies function
4. if gravity bool is set, they have gravity
5. if acceleration bool is set, accel
6. if collision bool is set, collide

// physical space, private structure
typedef struct Physics_S Physics;

void physics_init(Physics* space);
void physics_set_steps(Physics *space, int steps);
void physics_do_step(Physics *space);

// physical body
typedef struct Body_S
{
	struct Entity_S* owner;

	Vec3D position;
	Vec3D velocity;
	Vec3D posFixVector; 

	Cube bounds;
	
	MglCallback touch;

	bool needsFixing; 
	bool done;
}Body;

void physics_add_body(Body* body);
void physics_remove_body(Body* body);
static void physics_collision(Body* body);

// physical bounding shape
typedef struct Cube_S
{
    float x,y,z;
    float w,h,d;
}Cube;

#define cube_set(cube,a,b,c,l,m,n) (cube.x=a,cube.y=b,cube.z=c,\
                                    cube.w=l,cube.h=m,cube.d=n)
int cube_cube_intersection(
	Cube a, 
	Cube b);

*/

#endif