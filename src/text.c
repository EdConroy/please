#include "graphics.h"
#include "text.h"
#include "types.h"
#include "simple_logger.h"

Sprite* ascii;

extern GLint	view[4];
extern GLdouble model[16];
extern GLdouble projection[16];

void load_ascii()
{
	ascii = sprite_load("resources/text/some_ascii.png", 8192, 32);
}

void OMGAboutToDrawThisShytLIVE(char* ch, int length, int x, int y)
{
	// What's that? Can't use SDL_Rect cuz I'm in GL?
	// ILL JUS MAKE MY OWN
	// HAHA
	// DIDN'T EXPEXT THAT DID YA?
	// TWIWST
	rectangle src;
	Vec3DX pos;
	int ch_value;
	float left, right, top, bottom; // dont know WTF this means, will mess with lateer

	if (!ascii)
	{
		slog("go scrwe yourse;lf");
		return;
	}

	// no depth, this is 2D
	glDisable(GL_DEPTH_TEST);

	// texture mode, this is 2D
	glEnable(GL_TEXTURE_2D);

	// WTF IS COLOR TRACKING?
	glEnable(GL_COLOR_MATERIAL);

	// apparenlty this enables transparecy
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// alpha calculation?
	glEnable(GL_BLEND);

	// setting the current color
	glColor4f(1, 1, 1, 1);

	ch_value = (int)ch[0];

	
	src.x = (float)ch_value/256.0f * ascii->image->w;
	src.y = (float)ch_value/256 * ascii->image->h;

	src.w = ascii->image->w + src.x;
	src.h = ascii->image->h + src.y;

	// mapping window to object coord
	windowToGL(x, y, 0.99, model, projection, view, &pos.x, &pos.y, &pos.z);
	
	glBindTexture(GL_TEXTURE_2D, ascii->texture);

	/*
	//still don't know what this means
	left = src.x;
	right = src.w;
	top = src.y;
	bottom = src.h;
	*/

	// hnnnnnnnggghhh
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z); // go to this position
	glRotatef(0, 0.0f, 0.0f, 1.0f);
	glScalef(1, 1, 1);
	glTranslatef(ascii->x3D * 0.5f, ascii->y3D * 0.5f, 0.0f); // don't know why i need this
	glBegin(GL_QUADS);
	
	/*
	glTexCoord2f(left, top);
	glVertex3f(-ascii->x3D/2, -ascii->y3D * 0.5f, 0.0f);

	glTexCoord2f(left, bottom);
	glVertex3f(-ascii->x3D/2, ascii->y3D/2, 0.0f);

	glTexCoord2f(right, bottom);
	glVertex3f(ascii->x3D/2, ascii->y3D/2, 0.0f);

	glTexCoord2f(right, top);
	glVertex3f(ascii->x3D/2, -ascii->y3D/2, 0.0f);
	*/

	glEnd();

	glPopMatrix();
	glColor4f(1,1,1,1);
	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void drawToTheFrigginScreen(Vec2D size, Vec2D pos, GLuint tex_id)
{
	Vec2D verts[4]; // points on the screen
	Vec2D UVs[4];	// coordinates on the texture to draw
	int i;

	i = 1;

	// upper right
	verts[0].x = size.x/2;
	verts[0].y = size.y/2;

	// will attempt to change texture coordinates later
	UVs[0].x = 1;
	UVs[0].y = 0;

	// upper left
	verts[1].x = -size.x/2;
	verts[1].y = size.y/2;

	UVs[1].x = 0;
	UVs[1].y = .1;

	// bottom left
	verts[2].x = -size.x/2;
	verts[2].y = -size.y/2;

	UVs[2].x = 0;
	UVs[2].y = 0;

	// bottom right
	verts[3].x = size.x/2;
	verts[3].y = -size.y/2;

	UVs[3].x = .1;
	UVs[3].y = .1;

	// 2D
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	// maybe orig. method didn't work
	// because I didn't make a new ortho camera
	gluOrtho2D(-1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(pos.x, pos.y, 0);
	glRotatef(0, 0, 0, 1);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	// or mayb the tex id was wrong
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

//#define MAX_LENGTH_OF_TEXT 256

	//source.x = (num * TILE_WIDTH) % m_textureTile.Width();
	//source.y = (num * TILE_WIDTH) / m_textureTile.Width() * TILE_HEIGHT;
	//source.w = TILE_WIDTH;
	//source.h = TILE_HEIGHT;

// ascii value / total ascii char = position to start

// make a text object
// look thru entire char array
// find it in spritesheet
// save it as a sprite
// '0' + 9 = '9'

// position to start + 1 / 256 = position to end
// start, 0 end, 0
// start, 1  end, 1
/* 
void func (char *text)
{
	for (i)
	{
		int num = char[i];
		position to start = num/256;
		position to end = position to start + 1 / 256

		50 sprites onscreen
		50/2 for one side
		1/25 = 0.04
		translate by 0.4 after every draw
		push pop matrix

		glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture->texture);

		glBegin(GL_TRIANGLES);

		given pos << glTranslatef
																					(0, 0)   (.04, 0)  (.04, .08) < 1 triangle  (0, .08)



		Vec3D.x = 0
		Vec3D.y = .04
		Vec3D.z = 0.1 // so the camera can see it

		push MATRIX
		translate to given posiiton
		every positoin after acts like a child position
		draw thru array
		pop matrix

		create array
		Vec3D verts = { (0. .08)  (0, 0)  (.04, .08) < 1st triangle (0, 0)  (0.4, 0)  (.04, .08) < 2nd triangle }
		Vec3D which verts to draw = {1, 0, 2, 0, 3, 2)
		Vec2D texels = {start, 0
						end, 0
						start, 1
						end, 1}


		for (i = 0; i < length of array of which vertex to draw, i++)
		{
			triangle = which verts to draw[i]
			glTexCoord2f(trianlge->texels[i], triangle->texels[i++]);
			glVertex3f(triangle->verts[i].x, triangle->verts[i].y, triangle->verts[i].z);
		}

	}
}

*/
/*
void showText(char* text, int length)
{
	// need to get every char element individually to draw on screen
	int character;
	int i, j;
	// need array of triangles
	// each triangle has own verts and UVs
	int tri_array[MAX_LENGTH_OF_TEXT];

	float upperLeft; // are we using this as a box?
	
	// for each 4 coordinates
	for (j = 0; j < length; j++)
	{
		for (i = 0; i < 4; i++)
		{
			// printing i's texels and verts to screen
			tri_array[j]
		}
	}
}*/