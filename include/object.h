#ifndef __OBJECT_H__
#define __OBJECT_H__
/**
 * obj, model
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */

#include <SDL.h>
#include <GL\glew.h>
#include <stdlib.h>
#include "sprite.h"
#include "vector.h"

/**
 * @purpse reorganized version of headers with ability to load/parse object files into the structures
 */

// model is primary in entity struct

typedef struct TriPoint_S
{
    GLuint v;   /**<vertex index*/
    GLuint n;   /**<normal index*/
    GLuint t;   /**<texel index*/
}TriPoint;

typedef struct
{
    TriPoint p[3];
}ObjTriangle;

/**
  * A structure to represent simple 3D objects (ex. cubes)
  */
typedef struct Obj_S
{
	char		filename[512];

	Uint32		num_tris;
    Uint32		num_vertices;
    Uint32		num_texels;
    Uint32		num_normals;
    
    float		*vertex_array;
    float		*texel_array;
    float		*normal_array;
    
	ObjTriangle *triangle_array;
    
    Uint8		inuse;

}Obj;

/**
 * @brief initializes object subsystem
 */
void obj_init_all();

/**
 * @brief load an object file into an obj
 * @param filename the object file to parse
 * @return NULL on failure or an Obj pointer otherwise;
 */
Obj *obj_load(char *filename);

/**
 * @brief free an object from memory if no longer in use
 * @param obj the object to free
 */
void obj_free(Obj *obj);

/**
 * @brief draw an object primitive
 * @param obj the obj to draw
 * @param position the position to draw it at
 * @param rotation the yaw, pitch and roll of the obj to draw
 * @param scale the x,y,z stretch of the obj.  1,1,1 is no deformation
 * @param color the r,g,b,a value for the model as a whole
 * @param texture [optional] if provided, render with texture
 */
void obj_draw(
    Obj *obj,
    struct Vec3D_S position,
    struct Vec3D_S rotation,
    struct Vec3D_S scale,
    struct Vec4D_S color,
    struct Sprite_S *texture
);

///////////////////////////////////////////////////////////////////////

/**
  * A structure to represent 3D models (ex. human)
  */
typedef struct Model_S
{
	char		filename[512];

	Uint32		num_tris;
    Uint32		num_vertices;

	GLuint vertex_bo; // ???
    GLuint face_eab;  // ???

	float *vertex_array;    /**<vertex positions*/
    float *attribute_array; // ???

	GLuint *triangle_array; // why not use ObjTriangle

	Sprite *texture;        /**<pointer to texture data*/
    Uint8 used;             /**<refcount*/

}Model;

/**
 * @brief initialize the model system
 */
void model_init_all();

/**
 * @brief get a new model pointer
 * @return NULL on error or out of space, a zero initialized model pointer otherwise
 */
Model *model_init();

/**
 * @brief load an object file into a model
 * @param filename the object file to parse
 * @return NULL on failure or a model pointer otherwise;
 */
Model *obj_load_model(char *filename);

/**
 * @brief frees a loaded model
 * @param model a pointer to the model to free
 */
void model_free(Model *model);

/**
 * @brief checks if the model file is already loaded and returns a pointer to it if it is
 * @param filename the file to check if its already loaded
 * @return NULL on error or not loaded, the model pointer otherwise
 */
Model *model_get_by_filename(char *filename);

/**
 * @brief assign a texture to the model
 * @param model the model to assign to
 * @param texture the name of the texture file to load
 */
void model_assign_texture(Model *model,char *texture);

/**
 * @brief allocate the data needed for the triangle array buffer
 * NOTE: a NO-OP is it already exists
 * @param model the model to allocate data for
 * @param triangles how many triangles to allocate, must be > 0
 * @return -1 on error or 0 otherwise
 */
int model_allocate_triangle_buffer(Model *model, GLuint triangles);

/**
 * @brief allocate the data needed for the attribute array buffer
 * NOTE: a NO-OP is it already exists
 * @param model the model to allocate data for
 * @param attributes how many attributes to allocate, must be > 0
 * @return -1 on error or 0 otherwise
 */
int model_allocate_attribute_buffer(Model *model, GLuint attributes);

/**
 * @brief allocate the data needed for the vertex array buffer
 * NOTE: a NO-OP is it already exists
 * @param model the model to allocate data for
 * @param vertices how many vertices to allocate, must be > 0
 * @return -1 on error or 0 otherwise
 */
int model_allocate_vertex_buffer(Model *model, GLuint vertices);

#endif