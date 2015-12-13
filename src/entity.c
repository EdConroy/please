#include <stdio.h>
#include "entity.h"
#include "phys.h"
#include "simple_logger.h"
#include "level.h"
#include "game.h"

// list of entities
Entity* __entity_list = NULL;

// MAXIMUM AMOUNT OF ENITIES, TO BE SET
static int __entity_max = 0;

// AMT OF ENTITIES INIT'D
static int __entity_initialized = 0;

// private declaration: weapon
Vec3D weap_offset; // for the camera
Vec3D offset; // sum of player position and weap_offset

// private function: weapon 
void weapon_setup(Entity* ent); /**<sets up all the weapons in the inventory*/

// extern: game
extern float game_TimeRate; /**<how fast everything in the game runs*/

// glitch fix, to go in body.c hopefully; having problems with that file
void free_body(Body* body)
{
	body = NULL;
}

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

// pull a micheal bay on all the ents
void entity_deInit()
{
	int i;
	for (i = 0; i < __entity_max; i++)
	{
		if ( __entity_list[i].inuse)
		{
			ent_free(&__entity_list[i]); // free memory where entity is in use
			physics_remove_body(&__entity_list[i].body);
			free_body(&__entity_list[i].body);
		}
		free(__entity_list); // free entire entity list from memory
		__entity_max = 0;
		__entity_initialized = 0;
	}
}

// call all thinks, honestly there's only one right now
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
			ent_add_physics(&__entity_list[i].body);
		}
	}
}

// draw all entities
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

// initiate single entity
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

// draw a single entity
void ent_draw(Entity *ent)
{
	int i,j;

	float x, y, z, w, h, d;
	//temp

	int tris[36] = // i hsav no idab what this is
					{1,0,2,
					 2,3,1,
					 5,4,0,
					 0,1,5,
					 5,1,3,
					 3,7,5,
					 6,7,2,
					 7,3,2,
					 0,4,2,
					 4,6,2,
					 4,5,6,
					 5,7,6};

	Vec3D verts[8];

	if (!ent) return;
	
	if (strcmp(ent->name, "editor") == 0)
	{
		obj_draw
		(
		ent->model_array[ent->selected_model],
		ent->body.position, 
		ent->rot, 
		ent->scale, 
		ent->color, 
		ent->texture_array[ent->selected_model]
		);
	}else
	
	obj_draw
		(
		ent->model, 
		ent->body.position, 
		ent->rot, 
		ent->scale, 
		ent->color, 
		ent->texture
		);

	//if (game.showBoundingBox)

	/*
	// draw bounding box
	x = ent->body.bounds.x+ent->body.position.x;
	y = ent->body.bounds.y+ent->body.position.y;
	z = ent->body.bounds.z+ent->body.position.z;
	w = ent->body.bounds.w;
	h = ent->body.bounds.h;
	d = ent->body.bounds.d;

	verts[0] = vec3d(x, y+h, z);
	verts[1] = vec3d(x+w, y+h, z);
	verts[2] = vec3d(x, y, z);
	verts[3] = vec3d(x+w, y, z);
	verts[4] = vec3d(x, y+h, z+d);
	verts[5] = vec3d(x+w, y+h, z+d);
	verts[6] = vec3d(x, y, z+d);
	verts[7] = vec3d(x+w, y, z+d);

	glPushMatrix;

	glEnable(GL_BLEND);
    glColorMaterial(GL_FRONT,GL_DIFFUSE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	glRotatef(ent->rot.x, 1.0f, 0.0f, 0.0f);
    glRotatef(ent->rot.y, 0.0f, 1.0f, 0.0f);
    glRotatef(ent->rot.z, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);

		for(i=0;i<12;i++)
		{
			glColor4f(1.0f,0.0f,0.0f,0.25f);
			glVertex3f(verts[tris[3*i]].x,
			verts[tris[3*i]].y,
			verts[tris[3*i]].z);

			glColor4f(1.0f,0.0f,0.0f,0.25f);
			glVertex3f(verts[tris[3*i+1]].x,
			verts[tris[3*i+1]].y,
			verts[tris[3*i+1]].z);

			glColor4f(1.0f,0.0f,0.0f,0.25f);
			glVertex3f(verts[tris[3*i+2]].x,
			verts[tris[3*i+2]].y,
			verts[tris[3*i+2]].z);
		}
		glEnd();

		glBegin(GL_LINES);

		for(i=0;i<12;i++)
		{
			glColor4f(1.0f,1.0f,0.0f,1.0f);
			glVertex3f(verts[tris[3*i]].x,
			verts[tris[3*i]].y,
			verts[tris[3*i]].z);

			glColor4f(1.0f,1.0f,0.0f,1.0f);
			glVertex3f(verts[tris[3*i+1]].x,
			verts[tris[3*i+1]].y,
			verts[tris[3*i+1]].z);

			glColor4f(1.0f,1.0f,0.0f,1.0f);
			glVertex3f(verts[tris[3*i+1]].x,
			verts[tris[3*i+1]].y,
			verts[tris[3*i+1]].z);

			glColor4f(1.0f,1.0f,0.0f,1.0f);
			glVertex3f(verts[tris[3*i+2]].x,
			verts[tris[3*i+2]].y,
			verts[tris[3*i+2]].z);

			glColor4f(1.0f,1.0f,0.0f,1.0f);
			glVertex3f(verts[tris[3*i+2]].x,
			verts[tris[3*i+2]].y,
			verts[tris[3*i+2]].z);

			glColor4f(1.0f,1.0f,0.0f,1.0f);
			glVertex3f(verts[tris[3*i]].x,
			verts[tris[3*i]].y,
			verts[tris[3*i]].z);

		}
		glEnd();
		

	glDisable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
    */

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
					vec3d(.09f,.09f,.09f), 
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
	memset(ent->name, 0, sizeof(char)*128);
	ent->movetype = 0;
	physics_remove_body(&ent->inventory[0].body);
	memset(ent->inventory, 0, sizeof(Weapon)*3);
	physics_remove_body(&ent->body);
	ent->origin = vec3d(0,0,0);
	ent->accel = vec3d(0,0,0);
	ent->rot = vec3d(0,0,0);
	ent->scale = vec3d(0,0,0);
	ent->color = vec4d(0,0,0,0);
	ent->speed = 0;
	ent->gravity = 0;
	ent->nextThink = 0;
	ent->think = NULL;
	ent = NULL;
}

/*	this function was incorrectly named.
	it not only does acceleration, but does
	gravity as well as check if powerups are on */

void ent_add_physics(Body* body)
{
	body->velocity.x = body->owner->accel.x * 0.002;
	body->velocity.y = body->owner->accel.y * 0.002;
	
	if (body->done == 0) 
	{
		body->velocity.z -= body->owner->gravity * 0.0000002;
	}

	if (body->done == 1)
		body->velocity.z = 0;

	// bullet time
	if (!strcmp(body->owner->name, "player") == 0)
		vec3d_mult(body->velocity, body->velocity, game_TimeRate);
	
	vec3d_add(body->position, body->position, body->velocity);

}

/* THINK FUNCTIONS */
// set in entity create functions
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
Entity *ent_floor(Vec3D position, Vec3D rotation, const char *name, int gametype)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }
	
	ent->model = obj_load("resources/floortfix.obj");
	ent->texture = sprite_load("resources/seamless_tile_floor_1_by_ttrlabs-d4ojzlu.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
    cube_set(ent->body.bounds,-8,-8,-8,16,16,8);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_NONE; // floors don't move silly...
	ent->gravity = 0; // no gravity
    //mgl_callback_set(&ent->body.touch,touch_callback,ent);
	ent->rot = rotation;
	ent->body.owner = ent; // when I make the body, set the owner of that body to be the floor entity
	physics_add_body(&ent->body); // adding myself to the list of physical bodies
	return ent;
}

Entity *ent_player(Vec3D position, Vec3D rotation, const char *name, int gametype)
{
	Entity * ent;
	//int i;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }

	//ent->model = obj_load("resources/cube.obj"); // having problems with this...
    vec3d_cpy(ent->body.position, position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->movetype = MTYPE_PLAYER; // i am player, i move like player
	ent->gravity = 10;
	ent->health = 2000;
	ent->rot = rotation;
	physics_add_body(&ent->body);
	ent->body.owner = ent;

	weapon_setup(ent); // setup all my weapons. if commented out, no weapons

	return ent;
}

Entity *ent_editor(Vec3D position, const char *name)
{
	Entity * ent;
	ent = ent_init();
    if (!ent)
    {
        return NULL;
    }

	//ent->model = obj_load("resources/cube.obj");
    vec3d_cpy(ent->body.position, position);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    sprintf(ent->name,"%s",name);
	ent->body.owner = ent;

	//editor_inv_setup();
	ent->model_array[0] = obj_load("resources/cube.obj");
	ent->texture_array[0] = sprite_load("resources/cube_text.png",1024,1024);

	ent->model_array[1] = obj_load("resources/floortfix.obj");
	ent->texture_array[1] = sprite_load("resources/seamless_tile_floor_1_by_ttrlabs-d4ojzlu.png",1024,1024);

	ent->model_array[2] = obj_load("resources/cube.obj");
	ent->texture_array[2] = sprite_load("resources/metal_txt.png",1024,1024);

	ent->selected_model = 0; // starting model shpould be enemy cube

	return ent;
}

Entity *ent_obstacle(Vec3D position, Vec3D rotation, const char *name, int gametype)
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
	ent->movetype = MTYPE_ENT; // i am ent, i move like everybody else
	ent->gravity = 0;
	ent->origin = position; // save where i first spawn, gonna need it for the think function
	ent->rot = rotation;
	physics_add_body(&ent->body);
	ent->body.owner = ent;
	ent->think = thnk_back_forth; // when i spawn, i move back and forth

	return ent;
}

Entity *ent_projectile(Vec3D position, const char *name, int gametype)
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
	ent->accel.y = 1; // i accelerate constantly, because I am a projectile. this i do.
	ent->scale = vec3d(.1,.1,.1); // oh oh oh TOO BIG oh
	ent->origin = position;
	physics_add_body(&ent->body);
	ent->body.owner = ent;

	return ent;
}

/* temp spawn projectile */
void ShootProjectile(Entity* ent)
{
	Entity* proj = ent_projectile(ent->body.position, "projectile", PLAY_GAME);
}

/* flesh out this function later for level design, may transfer to level.h */
void CreateEntity(const char *name, Vec3D pos, Vec3D rot, int gametype)
{
	if (strcmp(name, "floor") == 0)
	{
		ent_floor(pos, rot, name, gametype);
	}

	if (strcmp(name, "player") == 0)
	{
		ent_player(pos, rot, name, gametype);
	}

	if (strcmp(name, "obstacle") == 0)
	{
		ent_obstacle(pos, rot, name, gametype);
	}
}

/** weapon **/ // weapon.h???
// called in ent_player which is called in game_Init()
void weapon_setup(Entity* ent)
{
	// this information would normally be put in a .txt file
	// hard coding because it's easier for me for now

	// this is for the camera so the weapons show up on the side of the player
	weap_offset = vec3d(0, -4.6, 0);
	
	// #knife
	ent->inventory[0].model = obj_load("resources/Knife.obj");
	ent->inventory[0].weaponType = WEAP_MELEE;
	ent->inventory[0].active = true;
	ent->inventory[0].maxAmmo = 10;
	ent->inventory[0].ammo = ent->inventory[0].maxAmmo;
	ent->inventory[0].cooldown = 100;
	ent->inventory[0].body.owner = ent;
	physics_add_body(&ent->inventory[0].body);
	
	// #gun
	ent->inventory[1].model = obj_load("resources/Cube.obj");
	ent->inventory[1].weaponType = WEAP_FIREARM;
	ent->inventory[1].active = false;
	ent->inventory[1].maxAmmo = 20;
	ent->inventory[1].ammo = ent->inventory[1].maxAmmo;
	ent->inventory[1].cooldown = 100;
	
	// #punch
	ent->inventory[2].model = obj_load("resources/cube.obj");
	ent->inventory[2].weaponType = WEAP_SHEILD;
	ent->inventory[2].active = false;
	ent->inventory[2].maxAmmo = 25;
	ent->inventory[2].ammo = ent->inventory[2].maxAmmo;
	ent->inventory[2].cooldown = 100;
}

// called in game_Poll()
void weap_switch(Entity* ent) 	/* Depending on what weapon I am, move to the next one in the inventory */
{
	int curWeap;

	for (curWeap = 0; curWeap < 3; curWeap++)
	{
		if (ent->inventory[curWeap].active)
		{
			ent->inventory[curWeap].active = !ent->inventory[curWeap].active;
			
			/* If I am at the end of my inventory, start at the beginning again */
			if (curWeap == 2)
				ent->inventory[0].active = !ent->inventory[curWeap++].active;
			else
				ent->inventory[curWeap++].active = !ent->inventory[curWeap++].active;
			return;
		}
	}
}

/* GET THE PLAYER */
Entity* Player()
{
	int i;

	for (i = 0; i < __entity_max; i++)
	{
		if(strcmp(__entity_list[i].name, "player") == 0)
		{
			return &__entity_list[i];
		}
	}

	slog("couldn't find player?????\n");
	return NULL;
}