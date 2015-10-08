#ifndef _GAME_H_
#define _GAME_H_

#include "bool.h"

/**
* @brief Running the game.
*/
typedef struct gamedata
{
	int		(*Run)(); /***<function pointer that will run the game> **/

}GameData;

int game_Init();

#endif