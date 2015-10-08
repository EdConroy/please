#include "game.h"

extern GameData game;
static int game_running;

int main (int argc, char *argv[])
{
	game_Init();
	do { game.Run(); } while (!game_running);
	return 0;
}