#include <glib.h>
#include <stdio.h>
#include "entity.h"
#include "phys.h"
#include "game.h"
#include "collisions.h"
#include "simple_logger.h"

// extern: game
extern GameData	game;

// extern: entity
extern Entity* player;

/*
physics.c
1. create body
2. add body to body list
3. update bodies function
4. if gravity bool is set, they have gravity
5. if acceleration bool is set, accel
6. if collision bool is set, collide
*/
//////////////////////////////////////////////////////
//////////		    Physical Space			//////////
//////////////////////////////////////////////////////

struct Physics_S
{
	GList *bodyList;	

	int steps;			
	int stepsTaken;		

	float stepFactor;
	Vec3D stepVector;

	//Vec3D gravity;		
};

void physics_free();

Physics* physics_init()
{
	Physics *space;

	space = (Physics *)calloc(1, sizeof(struct Physics_S));
	slog("Physics initiated");
	atexit(physics_free);
	return space;
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
    space->stepFactor = 1.0/steps; // speed in game doesn't seem constant
}

static void physics_collision(Physics *space, Body *body)
{
    GList *it;
    Cube a,b;
    Body *other;
    
	if ((!body) || (body->done))return;

	// if the owner of my body says i can't do this, don't do this
	if (!body->owner->canCollide)return;
    
    a.x = body->position.x + body->bounds.x;
    a.y = body->position.y + body->bounds.y;
    a.z = body->position.z + body->bounds.z;
    a.w = body->bounds.w;
    a.h = body->bounds.h;
    a.d = body->bounds.d;
    
	for (it = space->bodyList;it != NULL;it = g_list_next(it))
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
			// will probably start checking movetypes instead of classnames

			if (other->owner)
			{
				// if the player is not part of any collisions
				// set body->done = 0, so gravity will act
				// if (body_no_collision())
				if(strcmp(body->owner->name, "player") != 0)
					player->body.done = 0;

				// touch/callback 1 for floor collision
				// HitFloor() callback function
				if (strcmp(other->owner->name, "floor1") == 0)
				{
					if (strcmp(body->owner->name, "player") == 0)
					{
						body->done = 1;
						slog("%i", player->body.done);
					}
				}

				if (strcmp(other->owner->name, "floor2") == 0)
				{
					if (strcmp(body->owner->name, "player") == 0)
					{
						body->done = 1;
						//printf("boop2");
					}
				}

				// touch/callback 2 for #knife attack

				// All statements with inventory checks will go in
				// to one function called Attacked()
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

				// touch/callback 4 for throw attack
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
	if ((!body) || (!body->owner->canGravity)) return;

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


static void physics_checkpowerups(Physics *space, Body *body)
{
	if (!body) return;
	
	//vec3d_scale(space->stepVector, space->stepVector, 0.1);
}

// velocity/accel
static void physics_update(Physics *space)
{
    GList	*it;
	Body	*body;

    for (it = space->bodyList; it != NULL;it = g_list_next(it))
    {
        if (!it->data)continue;

		body = (Body *)it->data;

		// physics attribute modifiers
        physics_collision(space, body);

		physics_gravity(body);
		physics_acceleration(body);

		// applies specified physics time rate to all bodies
		vec3d_scale(space->stepVector,body->velocity,space->stepFactor);

		//vec3d_mult(space->stepVector, space->stepVector, 5);

		vec3d_add(body->position,body->position,space->stepVector);

		// applies physics to object with changes from the 4 func. if any
		//vec3d_add(body->position, body->position, body->velocity);

    }
}

void physics_do_step(Physics *space)
{
	GList *it;
    if (!space)return;
	if (space->stepsTaken == space->steps)
    {
        space->stepsTaken = 0;
		/*
		for (it = space->bodyList;it != NULL;it = g_list_next(it))
        {
            if (!it->data)continue;
            body_reset((Body *)it->data);
        }*/
    }
    
	physics_update(space);
    space->stepsTaken++;
}

void physics_free()
{
    if (!game.physics)return;
	// clear body list too
    free(game.physics);
	slog("physics closed");
}

void physics_add_body(Physics *space, Body* body)
{
	if (!space)return;
    if (!body)return;
	space->bodyList = g_list_append(space->bodyList,body);
}

void physics_remove_body(Physics *space, Body* body)
{
	if (!space)return;
    if (!body)return;
	space->bodyList = g_list_remove(space->bodyList,body);
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