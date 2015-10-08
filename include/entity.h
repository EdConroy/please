#ifndef _ENTITY_H_
#define _ENTITY_H_

#define MAX_ENT 255

typedef struct Entity_S
{
	int inuse;
	int refID;

	char name[128];

	struct Body_S* body;

	Vec3D accel;
	Vec3D rot;
	Vec3D scale;
	Vec4D color;

	Sprite* texture;

	/* entity has choice of either one */
	struct Obj_S* object;
	struct Model_S* model;
	
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
 * @brief creates an new floor
 * @param position for floor
 * @param name for floor
 */
Entity *newFloor(Vec3D position, const char *name);

/**
 * @brief draws an entity
 * @param ent the entity to draw
 */
void ent_draw(Entity* ent);

/**
 * @brief frees entity from memory
 */
void ent_free(Entity* ent);

#endif