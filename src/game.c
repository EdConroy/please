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
	game.Run = Run;

	return 1;
}