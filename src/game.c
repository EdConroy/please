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

Entity* floor1;
Entity *floor2, *floor3, *obstacle1;
Entity* player;

GList *it;

extern GList* __bodyList;

int curMouseX, curMouseY;

// private declarations: camera
Vec3D camera_position;
Vec3D camera_rotation;

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
                 -position.y+5,
                 -position.z);
}

// private definitions
void game_Poll()
{
	int mouseX, mouseY;
	int horiz, verti;

	curMouseX = 0;
	curMouseY = 0;

	while (SDL_PollEvent (&events))
	{
		//switch (events.type)
		
		if (events.type == SDL_QUIT)
			game_running = false;
		if (events.type == SDL_KEYDOWN)
			{
				switch(events.key.keysym.sym)
				{
				case SDLK_w:
						{
						player->accel.y = 4;
						break;
						}
						case SDLK_s:
						{
						player->accel.y = -4;
						break;
						}
					case SDLK_a:
						{
						player->accel.x = -4;
						break;
						}
					case SDLK_d:
						{
						player->accel.x = 4;
						break;
						}
				}
			}
		if (events.type == SDL_KEYUP)
			{
				switch(events.key.keysym.sym)
				{
						case SDLK_w:
						{
						player->accel.y = 0;
						break;
						}
						case SDLK_s:
						{
						player->accel.y = 0;
						break;
						}
					case SDLK_a:
						{
						player->accel.x = 0;
						break;
						}
					case SDLK_d:
						{
						player->accel.x = 0;
						break;
						}
				}
			}
		if (events.type == SDL_MOUSEMOTION)
		{
			//mouseOnCamera();
			SDL_GetMouseState(&mouseX, &mouseY);

			horiz = ((curMouseX - mouseX) - (512) * .00000000000000001);
			verti = ((curMouseY - mouseY) - (384)) * .9;

			//printf("%f\n", player->rot.x);

			player->rot.z = horiz;
			player->rot.x = verti;

			if (player->rot.x <= -650)
				player->rot.x = -650;
			if (player->rot.x >= -610)
				player->rot.x = -610;

			if (player->rot.z >= -300)
				player->rot.z = -300;
			if (player->rot.z <= -400)
				player->rot.z = -400;

			curMouseX = mouseX;
			curMouseY = mouseY;
		}
	}
}

void game_Update()
{
	ent_add_gravity(floor1);
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
	set_camera(player->body.position, player->rot); // gotta make a position offset
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

	// level layout "loadTestLevel();"
	floor1 = ent_floor(vec3d(0,0,0), "floor1");
	floor1->rot = vec3d(90,0,0);

	floor2 = ent_floor(vec3d(15.5, 0, 0), "floor2");
	floor2->rot = vec3d(90, 0, 0);
	
	player = ent_player(vec3d(0,0,10), "player");
	player->rot = vec3d(80,0,0);

	obstacle1 = ent_obstacle(vec3d(5, 0, 1), "obstacle1");

	slog("game initialization finished");
	return 1;
}

void game_Exit()
{
	// deInit everything here
	game.Run = NULL;
}