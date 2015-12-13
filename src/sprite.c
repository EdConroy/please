#include <SDL_image.h>
#include "simple_logger.h"
#include <stdlib.h>
#include "graphics.h"
#include "sprite.h"

#define MAXSPRITES	1024

static Sprite SpriteList[MAXSPRITES];

extern GLint	view[4];
extern GLdouble model[16];
extern GLdouble projection[16];


void sprite_init_all()
{
	int x;
	memset(SpriteList,0,sizeof(Sprite) * MAXSPRITES);
	for(x = 0;x < MAXSPRITES;x++)SpriteList[x].image = NULL;
	atexit(sprite_close_all);
}

Sprite *sprite_init()
{
    int i;
    for (i = 0; i < MAXSPRITES; i++)
    {
        if (!SpriteList[i].used)
        {
            memset(&SpriteList[i],0,sizeof(Sprite));
            SpriteList[i].used = 1;
            return &SpriteList[i];
        }
    }
    return NULL;
}

Sprite *SpriteGetByFilename(char *filename)
{
    int i;
    for (i = 0;i < MAXSPRITES;i++)
    {
        if  ((SpriteList[i].used > 0)
            &&(strcmp(SpriteList[i].filename,filename) == 0))
        {
            return &SpriteList[i];
        }
    }
    return NULL;
}

Sprite *sprite_load(char *filename,int fw, int fh)
{
    Sprite *sprite;
    SDL_Surface *image;
    int Mode = GL_RGB;
    
    sprite = SpriteGetByFilename(filename);
    if (sprite)return sprite;
    
	sprite = sprite_init();
    if (!sprite)return NULL;
    
    image = IMG_Load(filename);
    if (!image)
    {
        slog("failed to load sprite image file: %s, re: %s",filename, SDL_GetError());
        return NULL;
    }
    sprite->image = image;
    if (fw == -1)
    {
        sprite->w = sprite->image->w;
    }
    if (fh == -1)
    {
        sprite->h = sprite->image->h;
    }
    /*set the rest of the data*/
    
	// i don't know why I need to do this
    windowToGL(
		((640 >> 1) + 64),
		((480 >> 1) + 32),
		0.99, model, projection, view,
		&sprite->x3D, &sprite->y3D, &sprite->z3D);

    // You should probably use CSurface::OnLoad ... ;)
    //-- and make sure the Surface pointer is good!
    glGenTextures(1, &sprite->texture);
    glBindTexture(GL_TEXTURE_2D, sprite->texture);

    if(sprite->image->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, sprite->image->w, sprite->image->h, 0, Mode, GL_UNSIGNED_BYTE, sprite->image->pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    return sprite;
}

void DeleteSprite(Sprite *sprite)
{
    if (!sprite)return;
    if (sprite->image != NULL)
    {
        SDL_FreeSurface(sprite->image);
    }
    memset(sprite,0,sizeof(Sprite));
}

void sprite_free(Sprite *sprite)
{
  /*first lets check to see if the sprite is still being used.*/
  if (!sprite)return;
  sprite->used--;
  if(sprite->used <= 0)
  {
      DeleteSprite(sprite);
  }
}

void sprite_close_all()
{
  int i;
  for(i = 0; i < MAXSPRITES; i++)
  {
      DeleteSprite(&SpriteList[i]);
  }
}