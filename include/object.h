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

/**
 * @purpse reorganized version of headers with ability to load/parse object files into the structures
 */

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


}Model;


#endif