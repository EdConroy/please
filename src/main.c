#include "game.h"
#include "graphics3.h"

extern GameData game;
int game_running = true;

int main (int argc, char *argv[])
{
	game_Init(); // start the game
	do { game.Run(); } while (graphx_windowOpen()); // keep running the game until "game_running" says not to
	game_Exit(); // exit the game when "game_running" = 0
	
	return 0;
}