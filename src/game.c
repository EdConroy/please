#include "graphics.h"
#include <stdio.h>
#include <SDL.h>
#include "simple_logger.h"
#include "game.h"
#include "entity.h"

GameData game;
SDL_Event events;

extern int game_running;

// private declarations
void game_Poll();
void game_Update();
void game_Draw();
int Run();

// private definitions
void game_Poll()
{
	while (SDL_PollEvent (&events))
	{
		switch (events.type)
		{
		case SDL_QUIT:
			game_running = false;
		}
	}
}

void game_Draw()
{
	graphics_clear_frame();
	graphics_next_frame();
}

// private definitions
int Run()
{
	slog("running: %d", SDL_GetTicks());
	
	game_Poll();
	// update
	game_Draw();
	
	return 1;
}

int game_Init()
{
	// init everything here
	game.Run = Run;

	if (graphics_init(640,480,1,"please",33) != 0)
	{
		slog("graphics didn't load up very well");
		return -1;
	}

	return 1;
}

void game_Exit()
{
	// deInit everything here
	game.Run = NULL;
}