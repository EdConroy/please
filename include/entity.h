#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "vector.h"
#include "object.h"
#include "body.h"

#define MAX_ENT 255

/* Structure for weapons */
typedef struct Weapon_S
{
	Obj*	model;
	Sprite*	texture;
	Body	body;

	int		ammo;
	int		maxAmmo;

	int		weaponType;
	
	float	cooldown;

	pbool	active;
	pbool	attack;
}Weapon;

/* Enumeratio for weapon types */
enum
{
	WEAP_FIREARM,
	WEAP_MELEE,
	WEAP_SHEILD // ended up being more of a punch

}WEAPONTYPE;

/* Basic structure for anything in space that interacts */
typedef struct Entity_S
{
	int		inuse;
	int		refID;

	char	name[128];

	int		movetype;	// how i should move
	
	Weapon	inventory[3];

	Body	body;

	bool	canGravity; // if i can activate gravity on this ent
	bool	canAccel;
	bool	canCollide;

	Vec3D	origin;
	Vec3D	accel;
	Vec3D	rot;
	Vec3D	scale;
	Vec4D	color;

	Sprite* texture;	/**<object texture*/
	Obj*	model;
	
	float	speed;		/**speed relative to time*/
	float	gravity;

	int		nextThink;
	void	(*think)(struct Entity_S* ent); // think function
	
}Entity;


/**
 * @brief initialize the entity sub system
 * @param max the maximum number of simultaneously supported entities.
 */
void ent_init_all(int max);

/**
 * @brief draws all active entities
 */
void ent_draw_all();

/**
 * @brief draws all active entities
 */
void ent_thnk_all();

/**
 * @brief get a pointer to a new entity
 * @return NULL on no more entities or error,  a valid entity pointer otherwise
 */
Entity* ent_init();

/**
 * @brief gives and entity's body physics
 */
void ent_add_gravity(Body* body);

/**
 * @brief boolean that checks if data that was sent to the function is an entity
 * @return true (1) if data is an entity, false (0) if not
 */
int ent_is_real(void* data);

/**
 * @brief creates an new floor
 * @param position for floor
 * @param name for floor
 */
Entity *ent_floor(Vec3D position, const char *name);

/**
 * @brief create teh player
 * @param initial position
 * @param name
 */
Entity *ent_player(Vec3D position, const char *name);

/**
 * @brief creates an obstacle
 * @param initial position
 * @param name
 */
Entity *ent_obstacle(Vec3D position, const char *name);

/**
 * @brief draws an entity
 * @param ent the entity to draw
 */
void ent_draw(Entity* ent);

/**
 * @brief frees entity from memory
 */
void ent_free(Entity* ent);

/**
 * @brief set what kind of entity it is
 * not used yet
 */
void ent_set_type(Entity* ent);

/*	THINK FUNC	*/
void thnk_back_forth(Entity* ent);
void thnk_push(Entity* ent);

/* WEAPON */
void weap_switch(Entity* ent);
void ShootProjectile(Entity* ent);

#endif