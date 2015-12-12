#include "types.h"

rectangle RectMake(float x, float y, float w, float h)
{
	rectangle temp;

	temp.x = x;
	temp.y = y;
	temp.w = w;
	temp.h = h;

	return temp;
}