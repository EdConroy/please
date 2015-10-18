#ifndef _GAME_H_
#define _GAME_H_

#include "vector.h"
#include "graphics.h"
#include "phys.h"
#include "bool.h"

/**
* @purpse Running the game. Private??
*/

typedef struct gamedata
{
	int		(*Run)(); /***<function pointer that will run the game> **/
	Physics	*physics;

}GameData;

// public functions: game
int game_Init();
void game_Exit();

// public functions: game, powerups
pbool		game_PausedTime(); // checks if time in game is paused
pbool		game_BulletTimed(); // checks bulletTime boolean
#endif