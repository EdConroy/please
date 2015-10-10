#ifndef __SPRITE__
#define __SPRITE__

#include <SDL.h>
#include <GL\glew.h>

#define MAXSPRITEFILENAME 512

typedef struct Sprite_S
{
	SDL_Surface *image;					/**<pointer to the actual image in memory*/
    GLuint texture;						/**<to bind to the entity*/
    char  filename[MAXSPRITEFILENAME];  /**<the name of the file, keep them short*/
    int   w, h;                         /**<the width and height of the frames of the sprites, not the file itself*/
    int   color1,color2,color3;         /**<if the file was palette swapped these are the colors that were changed*/
    Uint8 used;                         /**<used by the maintanenc functions to keep track of how many times a single sprite is being used*/
}Sprite;

void sprite_init_all();

void sprite_free(Sprite *img);

Sprite *sprite_load(char *filename,int sizex, int sizey);

void sprite_close_all();

#endif