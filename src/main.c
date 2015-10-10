#include "game.h"

extern GameData game;
static int game_running = true;

int main (int argc, char *argv[])
{
	game_Init();
	do { game.Run(); } while (game_running);
	game_Exit();
	
	return 0;
}