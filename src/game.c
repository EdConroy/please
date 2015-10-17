#include <stdio.h>
#include <SDL.h>
#include <glib.h>
#include "graphics.h"
#include "phys.h"
#include "simple_logger.h"
#include "game.h"
#include "entity.h"

GameData	game;
SDL_Event	events;

Entity*	floor1;
Entity*	floor2, *floor3, *obstacle1;
Entity*	player;

GList	*it;

// extern: game
extern int		game_running;

// extern: body
extern GList*	__bodyList;

// extern: entity/weapon
extern Vec3D	offset;

// private declarations: camera
int		curMouseX, curMouseY;
Vec3D	camera_position;
Vec3D	camera_rotation;

// private declarations: game_time
float		game_TimeRate;

pbool		game_TimePause; // boolean that determines time
pbool		game_IfPausedTime(); // checks if time in game is paused
void		game_SetPauseTime(); // ability to set time boolean

pbool		game_BulletTime; // boolean that determine bulletTime
pbool		game_IfBulletTime(); // checks bulletTime boolean
void		game_SetBulletTime(); // ability to set bulletTime


// private declarations: game
void		game_Poll();
void		game_Update();
void		game_Draw();
int			Run();

// needs offset of player's position
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

// if the game was paused by the player
pbool game_IfPausedTime()
{
	if (!game_TimePause)
		return false;
	return true;
}

void game_SetPauseTime()
{
	game_TimePause = !game_TimePause;
}

pbool game_IfBulletTime()
{
	if (!game_BulletTime)
		return false;
	return true;
}

void game_SetBulletTime()
{
	game_BulletTime = !game_BulletTime;
}

// defining what happens for player input
void game_Poll()
{
	int mouseX, mouseY;
	int horiz, verti;

	int i;

	i = 0;
	curMouseX = 0;
	curMouseY = 0;

	while (SDL_PollEvent (&events))
	{	
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
				case SDLK_z:
					{
						weap_switch(player);
						break;
					}
				case SDLK_0:
					{
						game_SetPauseTime();
						break;
					}
				case SDLK_1:
					{
						game_SetBulletTime();
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

			// get new positions
			// highly inaccurate
			horiz = ((((curMouseX - mouseX)) + (1024/2)));
			verti = ((curMouseY - mouseY) - (768/2)) * .8;

			// set new positions
			player->rot.z = horiz;
			player->rot.x = verti;

			// clamping
			if (player->rot.x <= -650)
				player->rot.x = -650;
			if (player->rot.x >= -610)
				player->rot.x = -610;

			//if (player->rot.z >= -300)
				//player->rot.z = -300;
			//if (player->rot.z <= -400)
				//player->rot.z = -400;

			// save old positions
			curMouseX = mouseX;
			curMouseY = mouseY;
		}

		if (events.type == SDL_MOUSEBUTTONDOWN)
		{
			if (player->inventory)
			{
				for (i = 0; i < 3; i++)
				{
					if (player->inventory[i].active)
					{
						if (player->inventory[i].weaponType == WEAP_MELEE)
						{
							// UseKnife();
							player->inventory[i].attack = true;
						}
					}
				}
			}
		}

		player->inventory[i].attack = false;
	}
}

void game_Update()
{
	// give gravity/accel
	ent_add_gravity(&player->body);

	// if player stopped time
	if (!game_IfPausedTime())
	{
		ent_thnk_all(); // all the functions can think
		ent_add_gravity(&obstacle1->body); // give obstacle the ability to move
	}
	
	if (game_IfBulletTime())
		game_TimeRate = .4;
	else
		game_TimeRate = 1;

	for (it = __bodyList; it != NULL; it = g_list_next(it))
		physics_collision((Body*) it->data);
}

void game_Draw()
{
	graphics_clear_frame();
	glPushMatrix(); // ???
	set_camera(player->body.position, player->rot); // gotta make a position offset
	ent_draw_all(); // also draws weapons
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

	game_TimeRate = 1;

	// level layout "loadTestLevel();"
	floor1 = ent_floor(vec3d(0,0,0), "floor1");
	floor1->rot = vec3d(90,0,0);

	floor2 = ent_floor(vec3d(15.5, 0, 0), "floor2");
	floor2->rot = vec3d(90, 0, 0);
	
	player = ent_player(vec3d(0,0,10), "player");
	player->rot = vec3d(80,0,0);

	obstacle1 = ent_obstacle(vec3d(5, 0, 1.6), "obstacle1");

	slog("game initialization finished");
	return 1;
}

void game_Exit()
{
	// deInit everything here
	game.Run = NULL;
}