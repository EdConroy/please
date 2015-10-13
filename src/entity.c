#include <stdio.h>
#include "entity.h"
#include "phys.h"
#include "simple_logger.h"

// LIST OF ENTITIES
static Entity* __entity_list = NULL;

// MAXIMUM AMOUNT OF ENITIES, TO BE SET
static int __entity_max = 0;

// AMT OF ENTITIES INIT'D
static int __entity_initialized = 0;

//private function: shuts down list of entities
static void entity_deInit();

void ent_init_all(int max)
{
    if (__entity_initialized)
    {
        slog("already initialized");
        return;
    }
    __entity_list = (Entity *)malloc(sizeof(Entity)*max);
    memset(__entity_list,0,sizeof(Entity)*max);
    __entity_max = max;
    __entity_initialized = 1;
    slog("initialized %i entities",max);
    atexit(entity_deInit);
}

static void entity_deInit()
{
	int i;
	for (i = 0; i < __entity_max; i++)
	{
		if ( __entity_list[i].inuse)
		{
			ent_free(&__entity_list[i]); // free memory where entity is in use
			physics_remove_body(&__entity_list[i].body);
		}
		free(__entity_list); // free entire entity list from memory
		__entity_max = 0;
		__entity_initialized = 0;
	}
}

void ent_draw_all()
{
	int i;

	for (i = 0; i < __entity_max; i++)
	{
		if(__entity_list[i].inuse)
		{
			ent_draw(&__entity_list[i]);
		}
	}
}

Entity* ent_init()
{
	int i;
	for (i = 0; i < __entity_max; i++)
	{
		if (!__entity_list[i].inuse)
		{
			// go to this spot in memory, put nothing in there, with the memory size of an entity
			// in c++, this would look like "__entity_list[i] = new Entity();"
			memset(&__entity_list[i], 0, sizeof(Entity));
			__entity_list[i].inuse = 1;
			vec3d_set(__entity_list[i].scale, 1, 1, 1);
			vec4d_set(__entity_list[i].color, 1, 1, 1, 1);
			return &__entity_list[i];
		}
	}
	return &__entity_list[i];
}

void ent_draw(Entity *ent)
{
	if (!ent) return;

	
	obj_draw
		(
		ent->model, 
		ent->body.position, 
		ent->rot, 
		ent->scale, 
		ent->color, 
		ent->texture
		);
	
}

int ent_is_real(void* data)
{
	if (!data) return false; // if sent data is empty
	if (!__entity_initialized) return false;
	if ((Entity*) data < __entity_list) return false; // if the memory address number given is less than anything in the entity list
	if ((Entity*) data >= (__entity_list + __entity_max)) return false; // if the mem. add. num. is past anything in the entity list
	return true;
}

void ent_free(Entity* ent)
{
	if (!ent)
	{
		slog("A null entity was passed in the free function");
		return;
	}

	ent->inuse = 0;
	//obj_free(ent->model);
	//sprite_free(ent->texture);
}

// use this in the case of an if-statement
void ent_add_gravity(Entity* ent)
{
	// acceleration and velocity
	ent->body.velocity.x += ent->accel.x * 0.00000002;
	ent->body.velocity.y += ent->accel.y * 0.00000002;
	
	if (ent->body.position.z > 2)
		ent->body.velocity.z -= ent->gravity * 0.00000002;
	else
		ent->body.velocity.z = 0;
	// vec3d_add(ent->body.velocity, ent->body.velocity, ent->accel);

	vec3d_add(ent->body.position, ent->body.position, ent->body.velocity);

}

Entity *ent_floor(Vec3D position, const char *name)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }
	
	ent->model = obj_load("resources/level2.obj");
	ent->texture = sprite_load("resources/mountain_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_NONE;
	ent->gravity = 0;
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	physics_add_body(&ent->body);
	return ent;
}

Entity *ent_player(Vec3D position, const char *name)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }
	
	ent->model = obj_load("resources/level2.obj");
	ent->texture = sprite_load("resources/mountain_text.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_ENT;
	ent->gravity = 15;
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	physics_add_body(&ent->body);
	ent->body.owner = ent;
	return ent;
}