#include "graphics.h"
#include "text.h"
#include "simple_logger.h"
#include <string.h>

// text is drawn by surfing thru a texture
// and cutting out what's needed
// that way, there's no reason why this shouldn't
// work with opengl2 or 4 since I'm not using SDL
// except for loading the texture

// doing some weird data hiding in here, for now....

// text list
Text __text_list[MAX_TEXT];

// temp
Sprite* ascii;

/* birth */
void load_ascii()
{
	ascii = sprite_load("resources/text/some_ascii_red.png", 8192, 32);
}

int textInit()
{
	// initiate list
	int x;
	memset(__text_list, 0, sizeof(Text) * MAX_TEXT);
	//for(x = 0; x < MAX_TEXT; x++) __text_list[x].data = "";
	atexit(textExit);

	// load the ascii table (horizontal spreadsheet)
	load_ascii();
	return 1; // for now
}

/* death/rebirth */
void free_ascii()
{
	// for some reason, when in edit mode, even though ascii should've been cleared
	// ascii is still showing and using the cube texture for some reason
	// must be treated seperately from the other textures
	DeleteSprite(ascii);
}

void textExit()
{
	int i;

	for (i = 0; i < MAX_TEXT; i++)
	{
		__text_list[i].draw = 0;
		__text_list[i].inuse = 0;
		memset(&__text_list[i].data, 0, sizeof(char));
		memset(&__text_list[i], 0, sizeof(Text));
	}

	free_ascii();
}

/* life */
Text* makeText(char* data, Vec2D pos, Vec2D size)
{
	int i;

	for(i = 0; i < MAX_TEXT; i++)
	{
		if (!__text_list[i].inuse)
		{
			memset(&__text_list[i], 0, sizeof(Text));

			__text_list[i].inuse = 1;
			strcpy(__text_list[i].data, data);
			__text_list[i].pos = pos;
			__text_list[i].size = size;

			return &__text_list[i];
		}
	}
}

void text_draw(Text* text)
{
	int i;
	int value;
	
	// for as long as the length of the 'string'
	for (i = 0; i < strlen(text->data); i++)
	{
		value = text->data[i];

		if (!value == '\0')
		{
			drawToTheFrigginScreen(text->pos, text->size, ascii->texture, text->data[i], i);
		}
	}
}

void text_draw2(char* text)
{
	int i;
	int value;
	
	// for as long as the length of the 'string'
	for (i = 0; i < strlen(text); i++)
	{
		value = text[i];

		if (!value == '\0')
		{
			drawToTheFrigginScreen(vec2d(0.1f, 0.94f), vec2d(0.08f, -0.08f), ascii->texture, text[i], i);
		}
	}
}


void text_draw_all()
{
	int i;

	for (i = 0; i < MAX_TEXT; i++)
	{
		if(__text_list[i].inuse && (__text_list[i].draw))
		{
			text_draw(&__text_list[i]);
		}
	}
}

// graphics_2d_draw() - > text_draw_all()
void drawToTheFrigginScreen(Vec2D pos, Vec2D size, GLuint tex_id, int value, int count)
{
	Vec2D verts[4]; // points on the screen
	Vec2D UVs[4];	// coordinates on the texture to draw
	
	// upper left (start, 1)
	verts[1].x = -size.x/2;
	verts[1].y = size.y/2;

	UVs[1].x = value/256.0f;
	UVs[1].y = 1;

	// upper right (end, 1)
	verts[0].x = size.x/2;
	verts[0].y = size.y/2;

	UVs[0].x = (value + 1)/256.0f;
	UVs[0].y = 1;

	// bottom left, (start 0)
	verts[2].x = -size.x/2;
	verts[2].y = -size.y/2;

	UVs[2].x = value/256.0f;
	UVs[2].y = 0;

	// bottom right (end, 0)
	verts[3].x = size.x/2;
	verts[3].y = -size.y/2;

	UVs[3].x = (value + 1)/256.0f;
	UVs[3].y = 0;


	/*for (i = 0; i <4; i++)
	{
		if(i % 3 == 0)
		{
			verts[i].x = 1;
		}else
			verts[i].x = -1;
		if (i < 2)
		{
			verts[i].y = 1;
		}else
			verts[i].y = -1;
	
	}*/


	// 2D
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	gluOrtho2D(-1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// might need to change distance from screen (z)
	pos.x = pos.x + (count/20.0f);
	glTranslatef(pos.x, pos.y, 0.1f);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	// transparecny
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Four Points, 2 Triangles
	// Some points will be drawn twice
	glBegin(GL_TRIANGLES);

	glTexCoord2f(UVs[0].x,UVs[0].y);
    glVertex2f(verts[0].x,verts[0].y);

	glTexCoord2f(UVs[1].x,UVs[1].y);
    glVertex2f(verts[1].x,verts[1].y);

	glTexCoord2f(UVs[2].x,UVs[2].y);
    glVertex2f(verts[2].x,verts[2].y);

	glTexCoord2f(UVs[0].x,UVs[0].y);
    glVertex2f(verts[0].x,verts[0].y);

	glTexCoord2f(UVs[2].x,UVs[2].y);
    glVertex2f(verts[2].x,verts[2].y);

	glTexCoord2f(UVs[3].x,UVs[3].y);
    glVertex2f(verts[3].x,verts[3].y);
        
    glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
}
