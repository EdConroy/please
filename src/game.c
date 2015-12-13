#include <stdio.h>
#include <string.h>
#include <math.h>
#include <SDL.h>
#include <glib.h>
#include "graphics.h"
#include "phys.h"
#include "simple_logger.h"
#include "shader.h"
#include "game.h"
#include "level.h"
#include "entity.h"
#include "text.h"

GameData	game;
SDL_Event	events;

//Entity*	floor1;
//Entity*	floor2, *floor3, *obstacle1;
Entity*	player;
Entity* editor;

GList	*it;

// extern: game
extern int		game_running; // is the game running?

// extern: body
extern GList*	__bodyList;

// extern: entity/weapon
extern Vec3D	offset;
extern Entity*	__entity_list;

// private declarations: camera
int		curMouseX, curMouseY;
Vec3D	camera_position;
Vec3D	camera_rotation;

// new stuff for camera
Vec3D	camera_offset = {0, -3, 2};
Vec3D	next_camera_pos;

// extern: text
extern Sprite* ascii;

// private declarations: game - time
float		game_TimeRate;

pbool		game_TimePause; // boolean that determines time
pbool		game_IfPausedTime(); // checks if time in game is paused
void		game_SetPauseTime(); // ability to set time boolean

pbool		game_BulletTime; // boolean that determine bulletTime
pbool		game_IfBulletTime(); // checks bulletTime boolean
void		game_SetBulletTime(); // ability to set bulletTime


// private declarations: game
pbool		game_SaveState(); // saves positions of all entities presently in the game
pbool		game_LoadState(); // loads positions of all entities that were saved

void		game_Poll();
void		game_Update();
void		game_Draw();
int			Run();

// reminder: needs offset of player's position, called in game_Draw()
void set_camera(Vec3D position, Vec3D rotation)
{
	glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);

	glTranslatef(-position.x,
                 -position.y+5,
                 -position.z);
}

// private definitions: game - time

// if the game was paused by the player, called in game_Update()
pbool game_IfPausedTime()
{
	if (!game_TimePause)
		return false;
	return true;
}

// called in game_Poll()
void game_SetPauseTime()
{
	game_TimePause = !game_TimePause;
}

// called in game_Update()
pbool game_IfBulletTime()
{
	if (!game_BulletTime)
		return false;
	return true;
}

// called in game_Poll()
void game_SetBulletTime()
{
	game_BulletTime = !game_BulletTime;
}

pbool game_SaveState()
{
	// open file for writing
	// for every entity in game, put in txt file
	// close file

	FILE* file;
	int i;
	file = fopen("state.txt", "w");

	for (i = 0; i < MAX_ENT; i++)
	{
		if (__entity_list[i].inuse)
		{
			// entity name
			fprintf(file, __entity_list[i].name);
			fprintf(file, "\n");
			// entity position
			fprintf(file, "%f\t", __entity_list[i].body.position.x);
			fprintf(file, "%f\t", __entity_list[i].body.position.y);
			fprintf(file, "%f\n", __entity_list[i].body.position.z);
			// entity think function
			if (__entity_list[i].think == thnk_back_forth)
			{
				fprintf(file, "thnk_back_forth\n");
			}
			else if (__entity_list[i].think == thnk_push)
			{
				fprintf(file, "thnk_push\n");
			}
			else
				fprintf(file, "no_think\n");

			fprintf(file, "-\n\n");
		}
	}

	fclose(file);

	return true;
}

pbool game_LoadState()
{
	// open file for parsing
	// for every entity in file, put in game
	// close file

	FILE* file;
	int i,j,k,no_match;
	float x,y,z;
	char buf[24], think[24], name[16];
	file = fopen("state.txt", "r");

	for (i = 0; i < MAX_ENT; i++)
	{
		no_match = 0;
		fscanf(file, "%s", buf);
		for (j = 0; j < 5; j++)
		{
			if ((buf != NULL) 
				&& (!strcmp(buf, "-") == 0) 
				&& (!strcmp(buf, "thnk_back_forth") == 0)
				&& (!strcmp(buf, "thnk_push") == 0)) // isolating everything and finding the entity name
			{
				// find the name
				if (strcmp(buf, __entity_list[j].name) == 0)
				{
					strcpy(name,__entity_list[j].name);

					// get the vector
					fscanf(file, "%f %f %f", &x, &y, &z);
					// get the think
					fscanf(file, "%s", think);
					// set the vec
					__entity_list[j].body.position = vec3d(x, y, z);
					
					// checking what the think func is
					if (strcmp(think, "thnk_back_forth") == 0)
					{
						// set the think
						__entity_list[j].think = thnk_back_forth;
					}
					else if (strcmp(think, "thnk_push") == 0)
					{
						__entity_list[j].think = thnk_push;
					}
				}
				else
					no_match++;
				
				// checking if there is an entity in the file, that is no longer in the entity list
				if (no_match == 5)
				{
					// recreate that entity, only works for obstacles now, since that is the only thing
					// that can be killed right now
					fscanf(file, "%f %f %f", &x, &y, &z);
					CreateEntity(name, vec3d(x,y,z), vec3d(0,0,0), PLAY_GAME);
				}
			}
		}
	}

	fclose(file);

	return true;
}

// defining what happens for player input
void game_Poll()
{
	int mouseX, mouseY;
	int horiz, verti;

	Entity* ent;
	Cube a,b;

	int i;

	i = 0;
	curMouseX = 0;
	curMouseY = 0;

	//game.gamestate = EDIT_GAME;

	while (SDL_PollEvent (&events))
	{	
		if (events.type == SDL_QUIT)
			game_running = false;

		if(game.gamestate == EDIT_GAME) // if im in level edit
		{
			if (events.type == SDL_KEYDOWN)
			{
				switch (events.key.keysym.sym)
				{
					case SDLK_w:
					{
						vec3d_add(
							editor->body.position,
							editor->body.position,
							vec3d(
							-sin(editor->rot.z * DEGTORAD),
								cos(editor->rot.z * DEGTORAD),
								0
							)); break;
					}
					case SDLK_s:
					{
						vec3d_add(
							editor->body.position,
							editor->body.position,
							vec3d(
							sin(editor->rot.z * DEGTORAD),
								-cos(editor->rot.z * DEGTORAD),
								0
							)); break;
					}
					case SDLK_d:
					{
						vec3d_add(
							editor->body.position,
							editor->body.position,
							vec3d(
								cos(editor->rot.z * DEGTORAD),
								sin(editor->rot.z * DEGTORAD),
								0
							)); break;
					}
					case SDLK_a:
					{
						vec3d_add(
							editor->body.position,
							editor->body.position,
							vec3d(
								-cos(editor->rot.z * DEGTORAD),
								-sin(editor->rot.z * DEGTORAD),
								0
							)); break;
					}

					case SDLK_2:
					{
						editor->body.position.z++;
						break;
					}

					case SDLK_x:
					{
						editor->body.position.z--;
						break;
					}

					case SDLK_LEFT:
					{
						if(editor->selected_model > 0)
							editor->selected_model--;
						break;
					}

					case SDLK_RIGHT:
					{
						if(editor->selected_model < 3)
							editor->selected_model++;
						break;
					}

					case SDLK_SPACE:
					{
						switch(editor->selected_model)
						{
							// placeObject();

							case 0:
							{
								CreateEntity("obstacle", editor->body.position, editor->rot, game.gamestate);
								break;
							}

							case 1:
							{
								CreateEntity("floor", editor->body.position, editor->rot, game.gamestate);
								break;
							}

							case 2:
							{
								// move the player
								Player()->body.position = editor->body.position;
							}
						}
						break;
					}

					case SDLK_BACKSPACE:
					{
						// removePlacedObject();

						for(i = 0; i < MAX_ENT; i++)
						{
							ent = &__entity_list[i];

							if(ent != editor && ent->inuse)
							{
							a.x = editor->body.position.x + editor->body.bounds.x;
							a.y = editor->body.position.y +	editor->body.bounds.y;
							a.z = editor->body.position.z + editor->body.bounds.z;

							a.w = editor->body.bounds.w;
							a.h = editor->body.bounds.h;
							a.d = editor->body.bounds.d;

							vec3d_cpy(b, ent->body.position);

							b.w = ent->body.bounds.w;
							b.h = ent->body.bounds.h;
							b.d = ent->body.bounds.d;

							if (cube_cube_intersection(a, b))
							{
								slog("%s would totes be gettin removed", ent->name);
								ent_free(ent);
							}
							}
						}
						break;
					}

					case SDLK_INSERT:
					{
						game.gamestate = PLAY_GAME;
						saveLevel("resources/map/level.def");
						game_Init();
						slog("boop");
						return;
					}
				}
			}
		}

		if(game.gamestate == PLAY_GAME)
		{
			if (events.type == SDL_KEYDOWN)
			{
				switch(events.key.keysym.sym)
				{
				case SDLK_w:
					{
						Player()->accel.y = 5;
						break;
					}
				case SDLK_s:
					{
						Player()->accel.y = -5;
						break;
					}
				case SDLK_a:
					{
						Player()->accel.x = -5;
						break;
					}
				case SDLK_d:
					{
						Player()->accel.x = 5;
						break;
					}
				case SDLK_z:
					{
						weap_switch(Player());
						slog("weapon swtiched");
						break;
					}
				case SDLK_x:
					{
						game_SaveState();
						break;
					}
				case SDLK_l:
					{
						game_LoadState();
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
				case SDLK_INSERT:
					{
						game.gamestate = EDIT_GAME;
						mapEditorSetup();
						//slog("boop");
						return;
					}
				}
			}

			if (events.type == SDL_KEYUP)
			{
				switch(events.key.keysym.sym)
				{
					case SDLK_w:
					{
						Player()->accel.y = 0;
						break;
					}
					case SDLK_s:
					{
						Player()->accel.y = 0;
						break;
					}
					case SDLK_a:
					{
						Player()->accel.x = 0;
						break;
					}
					case SDLK_d:
					{
						Player()->accel.x = 0;
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
				Player()->rot.z = horiz;
				Player()->rot.x = verti;

				// clamping
				if (Player()->rot.x <= -650)
					Player()->rot.x = -650;
				if (Player()->rot.x >= -610)
					Player()->rot.x = -610;

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
				if (Player()->inventory)
				{
					for (i = 0; i < 3; i++)
					{
						// checking the weapon i am currently using with the active boolean (pbool)
						if (Player()->inventory[i].active)
						{
							// ActivateKnife(); to be put in player files or weapon files
							if (Player()->inventory[i].weaponType == WEAP_MELEE)
							{
								// i can attack with the knife 
								// moving the knife happens in ent_draw for now
								// the attack is checked in physics

								Player()->inventory[i].attack = true;
								//#knife
							}

							//ActivateGun(); 
							if( Player()->inventory[i].weaponType == WEAP_FIREARM)
							{
								// i can attack with this weapon 
								// the attack is checked in physics

								Player()->inventory[i].attack = true;
								ShootProjectile(Player());
								// #gun #firearm
							}

							// ActivateShield() #shield #rekt
							if( Player()->inventory[i].weaponType == WEAP_SHEILD)
							{
								// i can attack with this weapon 
								// the attack is checked in physics

								Player()->inventory[i].attack = true;
							}
						}
					}
				}
			}

			if (events.type == SDL_MOUSEBUTTONUP)
			{
				// SetOriginalPosition(); - set back to non-attack position
				Player()->inventory[0].attack = false;
				Player()->inventory[1].attack = false;
				Player()->inventory[2].attack = false;
			}
		}
	}
}

void game_Update()
{
	Vec3D rotation;
	int i;

	if (game.gamestate == PLAY_GAME)
	{
		// give entity physics ability, to be renamed body_add_physics()
		ent_add_physics(&Player()->body);

		// froze time
		if (!game_IfPausedTime())
		{
			ent_thnk_all(); // all the functions can think

			for (i = 0; i < MAX_ENT; i++)
			{
				// OOOOOOOOOMMMMMMMMGGGGGGGGG this is soooooooooooooooooooooooooooo temporaryyyyyyyy
				// if i'm a obstacle, I can have physics
				if (strcmp(__entity_list[i].name, "obstacle") == 0)
					ent_add_physics(&__entity_list[i].body);
			}

			//ent_add_physics(&obstacle1->body); // give obstacle the ability to move
		}
	
		// bullet time
		if (game_IfBulletTime())
			game_TimeRate = .4;
		else
			game_TimeRate = 1;
	}

	// check collisions, don't want things overlapping each other in edit mode
	for (it = __bodyList; it != NULL; it = g_list_next(it))
		physics_collision((Body*) it->data);

	if (game.gamestate == PLAY_GAME)
	{
		if (Player()->health <= 0)
		{
			Player()->health = 2000;
			game_LoadState();
		}
	}
	
	// mapEditorUpdate()
	if (game.gamestate == EDIT_GAME && editor)
	{
		camera_position = editor->body.position;
		rotation = vec_scale(vec3d(sin(camera_rotation.z * DEGTORAD),-cos(camera_rotation.z * DEGTORAD),0), 10);
		rotation.z = 5;
		// do the scale * 10 thing i did before, gives the camera an offset for the lvledit ent
		vec3d_add(camera_position, camera_position, rotation);
	}
}

void game_Draw()
{
	graphics_clear_frame(); // clear everything for the next draw
	glPushMatrix(); // save all current matrices and continue
	
	if (game.gamestate == PLAY_GAME) set_camera(Player()->body.position, Player()->rot); // gotta make a position offset // that doesn't work
	if (game.gamestate == EDIT_GAME) set_camera(camera_position, camera_rotation);
	
	// this is around where we would do the binary search to see what exactly needs to be drawn from the #bsp tree
	ent_draw_all(); // also draws weapons
	
	//OMGAboutToDrawThisShytLIVE("a",1, 32, 32);

	//ent_weap_draw();
	glPopMatrix();// reload previous matrices
	graphics_next_frame(); // show the next frame
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
	if (!game.hasLoaded)
	{
		// if game is already on, skip these steps
		game.Run = Run;

		init_logger("please_log.log");
		if (graphics_init(640,480,1,"please", 33) != 0)
		{
			slog("graphics didn't load up very well");
			return -1;
		}

		game_TimeRate = 1; // initial time rate
	}	

	entity_deInit();
	physics_clear_bodies();
	//body_super_clear();
	ent_init_all(255);
	obj_init_all();
	sprite_init_all();

	loadLevel("resources/map/level.def", "r");

	/*
	floor1 = ent_floor(vec3d(0,0,0), "floor1", PLAY_GAME);
	floor1->rot = vec3d(90,0,0);

	floor2 = ent_floor(vec3d(15.5, 0, 0), "floor2", PLAY_GAME);
	floor2->rot = vec3d(90, 0, 0);
	
	player = ent_player(vec3d(0,0,10), "player", PLAY_GAME);
	player->rot = vec3d(80,0,0);

	obstacle1 = ent_obstacle(vec3d(5, 0, 1.6f), "obstacle", PLAY_GAME);
	*/

	// ents are given different types depending on what mode their in
	// so copies aren't added to the map file

	load_ascii();

	slog("game initialization finished");
	return 1;
}

void game_Exit()
{
	// deInit everything here, don't call run anymore
	game.Run = NULL;
}