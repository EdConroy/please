#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "entity.h" // includes body.h, reduces errors

typedef enum
{
	MTYPE_NONE,	// movetype for teh floor 
	MTYPE_ENT	// movetype for all living entities, use gravity when jumped and/or in air

} movetype;

void physics_add_body(Body* body);
void physics_remove_body(Body* body);
void physics_collision(Body* body);

#endif