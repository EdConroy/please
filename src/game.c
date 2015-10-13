#include <stdio.h>
#include <SDL.h>
#include <glib.h>
#include "graphics.h"
#include "phys.h"
#include "simple_logger.h"
#include "game.h"
#include "entity.h"

GameData game;
SDL_Event events;

Entity* test;
Entity* player;

GList *it;

extern GList* __bodyList;

// private declarations: camera
Vec3D camera_position = {0,-10,0.3};
Vec3D camera_rotation = {90,0,0};

extern int game_running;

// private declarations: game
void game_Poll();
void game_Update();
void game_Draw();
int Run();

void set_camera(Vec3D position, Vec3D rotation)
{
	glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);

	glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

// private definitions
void game_Poll()
{
	while (SDL_PollEvent (&events))
	{
		switch (events.type)
		{
		case SDL_QUIT:
			game_running = false;
		case SDL_KEYDOWN:
			{
				if (events.key.keysym.sym == SDLK_SPACE)
					camera_position.y++;
			}

		}
	}
}

void game_Update()
{
	ent_add_gravity(test);
	ent_add_gravity(player);
	
	for (it = __bodyList; it != NULL; it = g_list_next(it))
	{
		physics_collision((Body*) it->data);
	}
}

void game_Draw()
{
	graphics_clear_frame();
	glPushMatrix(); // ???
	set_camera(camera_position, camera_rotation);
	ent_draw_all();
	glPopMatrix();// ???
	graphics_next_frame();
}

// private definitions
int Run()
{
	game_Poll();
	game_Update();
	game_Draw();
	
	return 1;
}

int game_Init()
{
	// init everything here
	game.Run = Run;

	init_logger("please_log.log");	// log for errors and such
	if (graphics_init(640,480,1,"please", 33) != 0)
	{
		slog("graphics didn't load up very well");
		return -1;
	}

	ent_init_all(255);

	test = ent_floor(vec3d(0,0,0), "test");
	test->rot = vec3d(100,0,0);
	
	player = ent_player(vec3d(0,0,10), "player");

	slog("game initialization finished");
	return 1;
}

void game_Exit()
{
	// deInit everything here
	game.Run = NULL;
}