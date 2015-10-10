#include <stdio.h>
#include <SDL.h>
#include "game.h"
#include "simple_logger.h"

GameData game;

// private declarations:
int Run();

// private definitions:
int Run()
{
	slog("running: %d", SDL_GetTicks());
	// pull
	// update
	// draw
	return 1;
}

int game_Init()
{
	// init everything here
	game.Run = Run;

	return 1;
}

bool game_Exit()
{
	// deInit everything here
	game.Run = NULL;
}