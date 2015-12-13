#include "body.h"	// vector.h is included in body.h

// extern: Entity
extern Entity* __entity_list;

void body_reset(Body *body)
{
    if (!body)return;
    body->needsFixing = 0;
    //body->done = 0;    
}

void body_process(Body *body)
{
    if (!body)return;
    if (body->needsFixing)
    {
        vec3d_add(body->position,body->position,body->posFixVector);
    }
}



void body_free(Body* body)
{
	body->owner = NULL;
	vec3d_set(body->velocity, 0,0,0);
	vec3d_set(body->position, 0,0,0);
	body = NULL;
}