#pragma once

#ifndef _BSP_H_
#define _BSP_H_

#include "vector.h"
#include "object.h"
#include "collisions.h"

#define BSP_L 0
#define BSP_R 1

/* binary space partition: used for world geometry, not individual obj #bsp */
typedef struct BSP_S
{
	Vec3D triangle[3];
	Vec3D normal;
	float d; // ???
	struct BSP_S *children[2]; // ref to child partitions
}BSP; // this is one node of the hypothetical tree

BSP *bsp_convert_from_obj(Obj *object); // convert obj into node onto tree
void bsp_save_to_file(BSP *bsp, char * filename);
BSP *bsp_load_from_file(char * filename);
pbool bsp_cube_collision(BSP *bsp,Cube *cube );
float bsp_ray_collision(BSP *bsp,Vec3D start, Vec3D end);

#endif