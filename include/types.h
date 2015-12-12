#ifndef __TYPES_H__
#define __TYPES_H__

#include <SDL.h>
#include <GL\glew.h>

// AND ON THE M(h)-PI(1/137)^8 * sqrt h/gth DAY...
// RECTANGLES ARE BORN
typedef struct
{
	GLfloat x,y,w,h;
}rectangle;

rectangle RectMake(float x, float y, float w, float h); // make rekt

#endif