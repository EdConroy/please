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

/* Enumeration for weapon types */
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

	char	name[64];
	char	classname[64];

	int		movetype;	// how i should move
	int		selected_model; // for map editor

	int		health;
	
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

	Obj*	model_array[MAX_ENT]; // for map editor
	Sprite* texture_array[MAX_ENT]; // also for map editor
	
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

void entity_deInit();

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
void ent_add_physics(Body* body);

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
Entity *ent_floor(Vec3D position, Vec3D rotation, const char *name, int gametype);

/**
 * @brief create teh player
 * @param initial position
 * @param name
 */
Entity *ent_player(Vec3D position, Vec3D rotation, const char *name, int gametype);

/**
 * @brief creates level editor placer that can place stuff in level
 * @param initial position
 * @param name
 */
Entity *ent_editor(Vec3D position, const char *name);

/**
 * @brief creates an obstacle
 * @param initial position
 * @param name
 */
Entity *ent_obstacle(Vec3D position, Vec3D rotation, const char *name, int gametype);

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

// create an enitity without a designated pointer
void CreateEntity(const char *classname, const char* name, Vec3D pos, Vec3D rot, int gametype);

/*	THINK FUNC	*/
void thnk_back_forth(Entity* ent); /** move back and forth **/
void thnk_push(Entity* ent); /** pushed **/

/* WEAPON */
void weap_switch(Entity* ent); /** switch to next weapon in inventory **/
void ShootProjectile(Entity* ent); /** initiate new projectile entity **/

/* GET THE PLAYER FROM ENTLIST */
Entity* Player();

#endif