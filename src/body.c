#include "body.h"	// vector.h is included in body.h

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
