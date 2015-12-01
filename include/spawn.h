#ifndef __SPAWN_H_
#define __SPAWN_H_

#include "vector.h"

typedef struct Spawn_S
{
	Vec3D position;
	Vec3D rotation;

	char id[3]; // concatenated onto potential spawnname
	char state[128]; // most likely will choose think function
	char target[128];
	char classname[128];
	char description[512];
	char tags[512];

}Spawn;

#endif