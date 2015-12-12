#ifndef __TEXT_H__
#define __TEXT_H__

#include "object.h"

// loads sprite
void load_ascii();

// failed
void OMGAboutToDrawThisShytLIVE(char* ch, int length, int x, int y);

void drawToTheFrigginScreen(Vec2D size, Vec2D pos, GLuint tex_id);

#endif