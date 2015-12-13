#include <stdio.h>
#include <string.h>
#include "simple_logger.h"
#include "level.h" //map.h
#include "entity.h"
#include "game.h"

// private declaraton: map (was level)
FILE *file;

// extern: game
extern Entity* floor1;
extern Entity* floor2;
extern Entity* player;
extern Entity* obstacle1;
extern Entity* editor;
extern GameData game;

// extern: game
extern Vec3D	camera_position;
extern Vec3D	camera_rotation;

int mapEditorSetup()
{
	// open a map
	//Map *map;

	/*map_new()  create new map for editing */
	
	//map->spawnCount = 0;
	//map->spawnList = NULL;
	//strcpy(map->mapname, "test_map");

	entity_deInit();
	ent_init_all(255);
	obj_init_all();
	sprite_close_all();
	sprite_init_all();

	camera_rotation.x = 80;
	camera_rotation.y = 0;
	camera_rotation.z = 0;

	// both this and the other setup will use the same file
	// one mode for editing, the other for playing

	slog("level initialization finished");

	// level layout 
	loadLevel("resources/map/test.def", "r");

	/*
	floor1 = ent_floor(vec3d(0,0,0), vec3d(90,0,0), "floor1", EDIT_GAME);
	floor1->rot = vec3d(90,0,0);

	floor2 = ent_floor(vec3d(15.5, 0, 0), vec3d(90,0,0), "floor2", EDIT_GAME);
	floor2->rot = vec3d(90, 0, 0);
	
	player = ent_player(vec3d(0,0,10), "player", EDIT_GAME);
	player->rot = vec3d(80,0,0);

	obstacle1 = ent_obstacle(vec3d(5, 0, 1.6f), "obstacle1", EDIT_GAME);
	*/

	editor = ent_editor(vec3d(0,0,0), "editor");

	/* open map file
	file = fopen ("resources/map/test_map.map", "w");
	if (file == NULL)
	{
		slog("wtf happened?");
		return 0;
	}*/

	return 1;
}

void editor_inv_setup()
{
	// use level editor entity, can change it's model to represent what object is selected for placing
	// that way when I search thru ent list to add stuff to file, there will be no extras

	// *loads a crapload of objects*

}

int loadLevel(char* filename, char* openType)
{
	FILE* file;
	char buffer[128];

	pbool end;

	char mapName[16];

	char classname[16];
	Vec3D pos;
	Vec3D rot;
	char think[24];

	end = false;

	file = fopen(filename, openType);

	rewind(file); // making sure we're at the begining of the file

	fscanf(file, "%*s %s", mapName);

	// while there's stuff in the file
	while(fscanf(file, "%s", buffer) != EOF && !end)
	{
		switch(buffer[0])
		{
			case '{':
			{
				fscanf(file, "%s", &classname);
				fscanf(file, "%*s %f %f %f", &pos.x, &pos.y, &pos.z); // %*s means it will be discarded; i want to skip over it and go straight to the numbers
				fscanf(file, "%*s %f %f %f", &rot.x, &rot.y, &rot.z); 
				fscanf(file, "%s", think);
				fscanf(file, "%*s"); // skip over end bracket

				CreateEntity(classname, pos, rot, game.gamestate);

				break;
			}

			case 'e':
			{
				if (strcmp("end", buffer) == 0)
				{
					end = true;
					fclose(file);
				}
			}
		}
	}

	return 1;
}