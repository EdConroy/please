#ifndef _GAME_H_
#define _GAME_H_

#include "vector.h"
#include "graphics.h"
#include "bool.h"

/***<for the game and the entities; defines what state the game/entity is in> **/
enum gameState
{
	PLAY_GAME,
	EDIT_GAME
};

/**
* @purpse Running the game. Private??
*/

typedef struct gamedata
{
	int		(*Run)(); /***<function pointer that will run the game> **/
	int		gamestate;
	int		hasLoaded; /*** <game successfully started> **/

}GameData;

int game_Init();
void game_Exit();

#endif