#ifndef __BODY_H__
#define __BODY_H__

#include "bool.h"

typedef struct Body_S
{
	struct Entity_S* owner;

	struct Vec3D_S position;
	struct Vec3D_S velocity;
	struct Vec3D_S posFix; /**<repositioning after collision*/

	struct Cube_S bounds;
	
	struct MglCallback_S touch;

	bool needsFix; /**<needs the position to be fixed*/
	bool done;
}Body;


void body_reset(Body *body);



#endif