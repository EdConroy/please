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

	struct Obj_S *object;
	struct Model_S* model;
	
}Entity;

void ent_init();
static void ent_close();
void ent_draw();

struct Entity_S* Init_Ent();
void KillEnt(struct Entity_S* ent);

void draw(struct Entity_S* ent);

void setPos (struct Entity_S* ent, float x, float y, float z);
void setSize (struct Entity_S* ent, float x, float y, float z);
//void setTexture (Entity* ent, Sprite* texture);

#endif