#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "mgl_callback.h"
#include "vector.h"
#include "collisions.h"

typedef enum
{
	MTYPE_NONE,	// movetype for teh floor
	MTYPE_PLAYER,
	MTYPE_ENT,	// movetype for all living entities, use gravity when jumped and/or in air
	MTYPE_PROJ

} movetype;
/*

new physics structure

1. create body
2. add body to body list
3. update bodies function
4. if gravity bool is set, they have gravity
5. if acceleration bool is set, accel
6. if collision bool is set, collide

*/
// physical space, private structure
typedef struct Physics_S Physics;
typedef struct Cube_S
{
    float x,y,z;
    float w,h,d;
}Cube;

Physics* physics_init();
void physics_set_steps(Physics *space, int steps);
void physics_do_step(Physics *space);

// physical body
typedef struct Body_S
{
	struct Entity_S* owner; /**<owner of this body*/

	Vec3D position;
	Vec3D velocity;
	Vec3D posFixVector;		/**<repositioning after collision*/

	Cube bounds;			/**<bounding box*/
	
	MglCallback touch;		/**<function to be called after specified event*/

	pbool needsFixing;		/**<if i need my position to be fixed*/	
	int done;
}Body;

/**
 * @brief resets a body pre-physics calculation
 * @param body the body to reset
 */
void body_reset(Body *body);

/**
 * @brief applies any post physics processing needed for the body
 * @param body the body to process
 */
void body_process(Body *body);

void physics_add_body(Physics *space, Body* body);
void physics_remove_body(Physics *space, Body* body);

/* physical bounding shape
typedef struct Cube_S
{
    float x,y,z;
    float w,h,d;
}Cube;
*/
#define cube_set(cube,a,b,c,l,m,n) (cube.x=a,cube.y=b,cube.z=c,\
                                    cube.w=l,cube.h=m,cube.d=n)
int cube_cube_intersection(
	Cube a, 
	Cube b);


#endif