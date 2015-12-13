#ifndef __BODY_H__
#define __BODY_H__

#include "vector.h"
#include "collisions.h"
#include "bool.h"
#include "entity.h"
#include "mgl_callback.h"

typedef struct Body_S
{
	struct Entity_S* owner; /**<owner of this body*/

	Vec3D position;
	Vec3D velocity;
	Vec3D posFixVector; /**<repositioning after collision*/

	Cube bounds; /**<bounding box*/
	
	MglCallback touch;  /**<function to be called after specified event*/

	bool needsFixing; /**<needs the position to be fixed*/
	bool done;
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

/**
 * @brief bodies keep causing a problem, will attempt to destroy them all here, must be called after clearing the glist
 */
//void body_super_clear();

/**
 * @brief free body from memory
 * @param body the body to delete
 */
void body_free(Body* body);

#endif