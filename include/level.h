#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "bsp.h"
#include "object.h"
#include "spawn.h"

typedef struct Map_S
{
	char mapname[128];
	Cube bounds; // how to use it so it doesn't act like a normal bbox?
	BSP *bsp;
	Obj *obj;
	Sprite *skybox;
	int spawnCount;
	Spawn *spawnList; // array of objects spawned
}Map;

int mapEditorSetup();
int loadLevel(char* filename, char* openType);
void editor_inv_setup();


#endif