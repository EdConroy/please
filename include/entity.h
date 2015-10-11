#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "vector.h"
#include "object.h"
#include "sprite.h"
#include "body.h"

#define MAX_ENT 255

typedef struct Entity_S
{
	int inuse;
	int refID;

	char name[128];

	Body body; //keep getting a disappearing red underline here

	Vec3D accel;
	Vec3D rot;
	Vec3D scale;
	Vec4D color;

	Sprite* texture;	/**<object texture*/

	float speed; // speed relative to time

	//Model* model; // treat same way SDL_Texture was treated
	Obj*	model;
	
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
 * @brief get a pointer to a new entity
 * @return NULL on no more entities or error,  a valid entity pointer otherwise
 */
Entity* ent_init();

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
 */
void ent_set_type(Entity* ent);

#endif