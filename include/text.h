#ifndef __TEXT_H__
#define __TEXT_H__

#include "object.h"
#include "collisions.h"

#define MAX_TEXT 64

typedef struct Text_S
{
	int inuse;
	int draw;
	char data[128]; // what needs to be drawn

	Vec2D pos, size;

}Text;

// opens ability to use text, loads horizontal ascii table
int textInit();
// ends aformentioned ability, frees ascii table from memory
void textExit();

// make a text, put in what you want it to say, also tell me how big it is
Text* makeText(char* data, Vec2D pos, Vec2D size);

void text_draw2(char* text);

void text_draw_all();

// working when it wants to
void drawToTheFrigginScreen(Vec2D pos, Vec2D size, GLuint tex_id, int value, int count);

#endif