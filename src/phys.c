#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "phys.h"
#include "collisions.h"
#include "simple_logger.h"

GList* __bodyList;

// subject to major changes in new_physics branch on git

// add bodies to physics
void physics_add_body(Body *body)
{
	if (!body) return;
	__bodyList = g_list_append(__bodyList, body);
}

// don't add bodies to physics
void physics_remove_body(Body *body)
{
	if (!body) return;
	__bodyList = g_list_remove(__bodyList, body);
}

void physics_clear_bodies()
{
	g_list_free(__bodyList);
	
	// THIS seems to fix the unhandled exception
	// read a forum, have to set pointer to NULL
	// "so glib knows it now represents an empty list
	// otherwise it's still pointing to where it was
	// before it was freed, which is now invalid"
	// even though, it's bad to do this because
	// everytime you change modes, glist gets new
	// memory address, that's bad

	__bodyList = g_list_alloc();
}

// check all collisions, calls touch functions
void physics_collision(Body *body)
{
	GList *it;

	Cube a,b; // 3d bounding boxes
	Body *other;

	if(!body)
		return;

	// i don't think the bodies are being correctly removed from memory
	if (!body->owner)
		return;

	// vec3d_cpy(a, a, body->position.x + body->bounds)
	a.x = body->position.x + body->bounds.x;
	a.y = body->position.y + body->bounds.y;
    a.z = body->position.z + body->bounds.z;

	a.w = body->bounds.w;
    a.h = body->bounds.h;
    a.d = body->bounds.d;

	for (it = __bodyList; it != NULL; it = g_list_next(it))
	{
		if (!it->data) continue;
		if (it->data == body) continue;

		// cast data as a body, because it is from __bodyList
		other = (Body*) it->data;

		vec3d_cpy(b, other->position);

		b.w = other->bounds.w;
        b.h = other->bounds.h;
        b.d = other->bounds.d;
		
		vec3d_add(b,b,other->bounds);

		// bounding box collision detection
		if (cube_cube_intersection(a, b))
		{
			if (other->owner)
			{
				// touch/callback 1 for floor collision
				if (strcmp(other->owner->classname, "floor") == 0)
					if (strcmp(body->owner->classname, "player") == 0)
					{
						body->done = 1;
					}

				// touch/callback 2 for #knife attack
				if (strcmp(other->owner->classname, "obstacle") == 0)
					if (strcmp(body->owner->classname, "player") == 0)
					{
						if (body->owner->inventory[0].attack)
							ent_free(other->owner);
						else
						{
							body->owner->health--;
							slog("player health: %d", body->owner->health);
						}
					}

				// touch/callback 3 for firearm attack
				if (strcmp(other->owner->classname, "obstacle") == 0)
					if (strcmp(body->owner->classname, "player") == 0)
					{
						if (body->owner->inventory[1].attack)
							ent_free(other->owner);
					}

				// touch/callback 3 for throw attack
				if (strcmp(other->owner->classname, "obstacle") == 0)
					if (strcmp(body->owner->classname, "player") == 0)
					{
						if (body->owner->inventory[2].attack)
							other->owner->think = thnk_push;
					}
			}
		}

	}
}

/*

physics.c
1. create body
2. add body to body list
3. update bodies function
4. if gravity bool is set, they have gravity
5. if acceleration bool is set, accel
6. if collision bool is set, collide

//////////////////////////////////////////////////////
//////////		    Physical Space			//////////
//////////////////////////////////////////////////////

struct Physics_S
{
	GList *bodyList;	

	int steps;			
	int stepsTaken;		

	float stepFactor;

	Vec3D gravity;		
};

void physics_free(Physics *space);

void physics_init(Physics* space)
{
	space = (Space *)calloc(1, sizeof(struct Physics_S));
	atexit(physics_free);
}

void physics_set_steps(Physics *space, int steps)
{
    if (!space)return;
    if (!steps)
    {
        slog("cannot support zero steps!");
        return;
    }
    space->steps = steps;
    space->stepFactor = 1.0/steps;
}

static void physics_collision(Physics *space, Body *body)
{
    GList *it;
    Cube a,b;
    Body *other;
    //Vec3D stepVector;
    //Vec3D stepOffVector;
    
    if ((!body) || (body->_done))return;

	// if the owner of my body says i can't do this, don't do this
	if (!body->owner->canCollide)return;
    
	// i do not understand
    //vec3d_scale(stepVector,body->velocity,space->stepFactor);
    //vec3d_negate(stepOffVector,stepVector);

	vec3d_add(body->position,body->position,stepVector);
    
    a.x = body->position.x + body->bounds.x;
    a.y = body->position.y + body->bounds.y;
    a.z = body->position.z + body->bounds.z;
    a.w = body->bounds.w;
    a.h = body->bounds.h;
    a.d = body->bounds.d;
    
    for (it = space->bodylist;it != NULL;it = g_list_next(it))
    {
        if (!it->data)continue;
        if (it->data == body)continue;

        other = (Body *)it->data;
        vec3d_cpy(b,other->position);
        b.w = other->bounds.w;
        b.h = other->bounds.h;
        b.d = other->bounds.d;
        vec3d_add(b,b,other->bounds);

        if (cube_cube_intersection(a,b))
        {
            // back the fuck off
            vec3d_cpy(body->_stepOffVector,stepOffVector);
            body->_done = 1;
            body->_needsBackoff = 1;
            if (body->touch.function)
            {
				// call touch functions
                body->touch.function(body->touch.data,other);
            }
        }
    }
}

void physics_collision(Body *body)
{
	GList *it;

	Cube a,b; // 3d bounding boxes
	Body *other;

	// vec3d_cpy(a, a, body->position.x + body->bounds)
	a.x = body->position.x + body->bounds.x;
	a.y = body->position.y + body->bounds.y;
    a.z = body->position.z + body->bounds.z;

	a.w = body->bounds.w;
    a.h = body->bounds.h;
    a.d = body->bounds.d;

	for (it = __bodyList; it != NULL; it = g_list_next(it))
	{
		if (!it->data) continue;
		if (it->data == body) continue;

		// cast data as a body, because it is from __bodyList
		other = (Body*) it->data;

		vec3d_cpy(b, other->position);

		b.w = other->bounds.w;
        b.h = other->bounds.h;
        b.d = other->bounds.d;
		
		vec3d_add(b,b,other->bounds);

		// bounding box collision detection
		if (cube_cube_intersection(a, b))
		{
			if (other->owner)
			{
				// touch/callback 1 for floor collision
				if (strcmp(other->owner->name, "floor1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						body->done = 1;
					}

				// touch/callback 2 for #knife attack
				if (strcmp(other->owner->name, "obstacle1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						if (body->owner->inventory[0].attack)
							ent_free(other->owner);
					}

				// touch/callback 3 for firearm attack
				if (strcmp(other->owner->name, "obstacle1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						if (body->owner->inventory[1].attack)
							ent_free(other->owner);
					}

				// touch/callback 3 for throw attack
				if (strcmp(other->owner->name, "obstacle1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						if (body->owner->inventory[2].attack)
							other->owner->think = thnk_push;
					}
			}
		}

	}
}


static void physics_gravity(Body *body)
{
	if ((!body || (!body->owner->canGravity)) return;

	// if in the air
	if (body->done == 0) 
		body->velocity.z -= body->owner->gravity * 0.0000002;

	// if "ground entity" = true
	if (body->done == 1)
		body->velocity.z = 0;

}

static void physics_acceleration(Body *body)
{
	if ((!body) || (!body->owner->canAccel)) return;

	body->velocity.x = body->owner->accel.x * 0.002;
	body->velocity.y = body->owner->accel.y * 0.002;
}


static void physics_checkpowerups(Body *body)
{
	if (!body) return;

	if (game_PausedTime()) 
		vec3d_scale(stepVector, stepVector, 0f)

	if (!game_BulletTimed()) 
		vec3d_scale(stepVector, stepVector, 0.1);
}

// velocity/accel
static void physics_update(Physics *space)
{
    GList	*it;
	Body	*body;

    for (it = space->bodylist;it != NULL;it = g_list_next(it))
    {
        if (!it->data)continue;

		body = (Body *)it->data;

		// physics attribute modifiers
        physics_collision(space, body);
		physics_gravity(body);
		physics_acceleration(body);

		// applies specified physics time rate to all bodies
		vec3d_scale(stepVector,body->velocity,space->stepFactor);
		vec3d_add(body->position,body->position,stepVector);

		physics_checkpowerups(body);

		// applies physics to object with changes from the 4 func. if any
		vec3d_add(body->position, body->position, body->velocity);

    }
    
 //	for (it = space->bodylist;it != NULL;it = g_list_next(it))
 //   {
 //       if (!it->data)continue;
 //       body_process((Body *)it->data);
 //   }
}

void physics_do_step(Physics *space)
{
	GList *it;
    if (!space)return;
    if (space->stepstaken == space->steps)
    {
        space->stepstaken = 0;
        
        for (it = space->bodylist;it != NULL;it = g_list_next(it))
        {
            if (!it->data)continue;
            body_reset((Body *)it->data);
        }
    }
    
    space_update(space);
    space->stepstaken++;
}

void physics_free(Physics *space)
{
    if (!space)return;
	// clear body list too
    free(space);
}

void physics_add_body(Physics *space, Body* body)
{
	if (!space)return;
    if (!body)return;
    space->bodylist = g_list_append(space->bodylist,body);
}

void physics_remove_body(Physics *space, Body* body)
{
	if (!space)return;
    if (!body)return;
    space->bodylist = g_list_remove(space->bodylist,body);
}

//////////////////////////////////////////////////////
//////////		    Physical Bodies			//////////
//////////////////////////////////////////////////////

void body_reset(Body *body)
{
    if (!body)return;
    body->needsFixing = 0;
    body->done = 0;    
}

void body_process(Body *body)
{
    if (!body)return;
    if (body->needsFixing)
    {
        vec3d_add(body->position,body->position,body->posFixVector);
    }
}

//////////////////////////////////////////////////////
//////////		    Physical Shape			//////////
//////////////////////////////////////////////////////

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

*/