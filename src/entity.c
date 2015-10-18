#include <stdio.h>
#include "entity.h"
#include "simple_logger.h"

// LIST OF ENTITIES
Entity* __entity_list = NULL;

// MAXIMUM AMOUNT OF ENITIES, TO BE SET
static int __entity_max = 0;

// AMT OF ENTITIES INIT'D
static int __entity_initialized = 0;

//private function: shuts down list of entities
static void entity_deInit();

// private declaration: weapon
Vec3D weap_offset;
Vec3D offset; // sum of player position and weap_offset

// private function: weapon
void weapon_setup(Entity* ent);

// extern: game
extern float game_TimeRate;

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
			//physics_remove_body(&__entity_list[i].body);
		}
		free(__entity_list); // free entire entity list from memory
		__entity_max = 0;
		__entity_initialized = 0;
	}
}

void ent_thnk_all()
{
	int i;

	for (i = 0; i < __entity_max; i++)
	{
		if(__entity_list[i].inuse && __entity_list[i].think)
		{
			__entity_list[i].think(&__entity_list[i]);
		}

		if(__entity_list[i].inuse && __entity_list[i].movetype == MTYPE_PROJ)
		{
			ent_add_gravity(&__entity_list[i].body);
		}
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
	int i,j;
	for (i = 0; i < __entity_max; i++)
	{
		if (!__entity_list[i].inuse)
		{
			// go to this spot in memory, put nothing in there, with the memory size of an entity
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
	int i;
	//temp

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

	// draw weapons to the screen
	for (i = 0; i < 3; i++)
	{
		if (ent->inventory)
		{
			// temp, to make weapon show up next to player
			vec3d_add(offset, ent->body.position, weap_offset);

			// #knife attack
			if (ent->inventory[i].attack)
				vec3d_add(offset, offset, vec3d(0,3,0));

			// show all weapons
			if (ent->inventory[i].active)
			{
			obj_draw
				(
					ent->inventory[i].model, 
					offset,
					vec3d(ent->rot.x, ent->rot.y, 0), 
					vec3d(.09,.09,.09), 
					ent->color, 
					ent->texture
				);
			}
		}
	}
	
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
	obj_free(ent->model);
	sprite_free(ent->texture);
}

/*	will be put in physics update function
	gives entity ability to move in world */
void ent_add_gravity(Body* body)
{
	//body->velocity.x = body->owner->accel.x * 0.002;
	//body->velocity.y = body->owner->accel.y * 0.002;
	
	//if (body->done == 0) 
	//{
		//body->velocity.z -= body->owner->gravity * 0.0000002;
	//}

	//if (body->done == 1)
		//body->velocity.z = 0;

	// bullet time
	//if (!strcmp(body->owner->name, "player") == 0)
		//vec3d_mult(body->velocity, body->velocity, game_TimeRate);
	
	//vec3d_add(body->position, body->position, body->velocity);

}

/* THINK FUNCTIONS */
void thnk_back_forth(Entity* ent)
{
	float bound1 = ent->origin.y;
	float bound2 = ent->origin.y + 5;

	if (ent->body.position.y <= bound1)
		ent->accel.y = 3;
	if (ent->body.position.y >= bound2)
		ent->accel.y = -3;
}

void thnk_push(Entity* ent)
{
	if(ent->accel.y >= -3)
		ent->accel.y = -18;
	if(ent->accel.y <= 3)
		ent->accel.y = 18;
}

/* CREATE ENTITIES */
Entity *ent_floor(Vec3D position, const char *name)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }
	
	ent->model = obj_load("resources/floortwo.obj");
	ent->texture = sprite_load("resources/seamless_tile_floor_1_by_ttrlabs-d4ojzlu.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,0,0,0,8,8,0.2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_NONE;
	ent->gravity = 0;
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	ent->body.owner = ent;
	//physics_add_body(&ent->body);
	return ent;
}

Entity *ent_player(Vec3D position, const char *name)
{
	Entity * ent;
	int i;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }

	//ent->model = obj_load("resources/cube.obj");
    vec3d_cpy(ent->body.position, position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_PLAYER;
	ent->gravity = 20;

	//physics_add_body(&ent->body);
	ent->body.owner = ent;

	weapon_setup(ent);

	return ent;
}

Entity *ent_obstacle(Vec3D position, const char *name)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }
	
	ent->model = obj_load("resources/cube.obj");
	ent->texture = sprite_load("resources/cube_text.png",1024,1024);
    vec3d_cpy(ent->body.position, position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_ENT;
	ent->gravity = 0;
	ent->origin = position;
	//physics_add_body(&ent->body);
	ent->body.owner = ent;
	ent->think = thnk_back_forth;

	return ent;
}

Entity *ent_projectile(Vec3D position, const char *name)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }
	
	ent->model = obj_load("resources/sphere.obj");
	ent->texture = sprite_load("resources/metal_txt.png",1024,1024);
    vec3d_cpy(ent->body.position, position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_PROJ;
	ent->gravity = 0;
	ent->accel.y = 1;
	ent->scale = vec3d(.1,.1,.1);
	ent->origin = position;
	//physics_add_body(&ent->body);
	ent->body.owner = ent;
	//ent->think = thnk_back_forth;

	return ent;
}

/* temp spawn projectile */
void ShootProjectile(Entity* ent)
{
	Entity* proj = ent_projectile(ent->body.position, "projectile");
}

/** weapon **/
void weapon_setup(Entity* ent)
{
	// this information would normally be put in a .txt file
	// hard coding because it's easier for me for now

	weap_offset = vec3d(0, -4.6, 0);
	// #knife
	ent->inventory[0].model = obj_load("resources/Knife.obj");
	ent->inventory[0].weaponType = WEAP_MELEE;
	ent->inventory[0].active = true;
	ent->inventory[0].maxAmmo = 10;
	ent->inventory[0].ammo = ent->inventory[0].maxAmmo;
	ent->inventory[0].cooldown = 100;
	//physics_add_body(&ent->inventory[0].body);
	ent->inventory[0].body.owner = ent;
	
	ent->inventory[1].model = obj_load("resources/Cube.obj");
	ent->inventory[1].weaponType = WEAP_FIREARM;
	ent->inventory[1].active = false;
	ent->inventory[1].maxAmmo = 20;
	ent->inventory[1].ammo = ent->inventory[1].maxAmmo;
	ent->inventory[1].cooldown = 100;
	
	ent->inventory[2].model = obj_load("resources/cube.obj");
	ent->inventory[2].weaponType = WEAP_SHEILD;
	ent->inventory[2].active = false;
	ent->inventory[2].maxAmmo = 25;
	ent->inventory[2].ammo = ent->inventory[2].maxAmmo;
	ent->inventory[2].cooldown = 100;
}

void weap_switch(Entity* ent)
{
	int curWeap;

	for (curWeap = 0; curWeap < 3; curWeap++)
	{
		if (ent->inventory[curWeap].active)
		{
			ent->inventory[curWeap].active = !ent->inventory[curWeap].active;
			
			if (curWeap == 2)
				ent->inventory[0].active = !ent->inventory[curWeap++].active;
			else
				ent->inventory[curWeap++].active = !ent->inventory[curWeap++].active;
			return;
		}
	}
}