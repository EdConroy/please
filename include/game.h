#ifndef _GAME_H_
#define _GAME_H_

#include "vector.h"
#include "graphics.h"
#include "bool.h"

/*
typedef struct Camera_S
{
	void (*setPosition)(Vec3D position);
	void (*setRotation)(Vec3D rotation);

}Camera;

/**
 * @brief sets functions for camera
 */
/*
void camera_Init();

/**
* @brief Running the game. Private??
*/

typedef struct gamedata
{
	int		(*Run)(); /***<function pointer that will run the game> **/

}GameData;

int game_Init();
void game_Exit();

#endif