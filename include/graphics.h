#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
/**
 * graphics3d
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

#define EPSILON   1e-6f // ???
#define TWOPI 6.283185308 // ???

/* conversion factor for converting from radians to degrees*/
#define RADTODEG  57.295779513082

/* conversion factor for converting from degrees to radians*/
#define DEGTORAD  0.017453292519943295769236907684886

/**
 * @brief initializes 3d graphics
 * @param sw the screen width you want
 * @param sh the screen height you want
 * @param fullscreen set true to initialize to fullscreen
 * @param project the name to appear on the game window
 * @param frameDelay the desired delay (in ms) between frames
 * @return -1 on error, 0 otherwise
 */
int graphics_init(int sw,int sh,int fullscreen,const char *project, Uint32 frameDelay);

/**
 * @brief get the active shader program index
 * @return 0 on error or the index of the active shader program
 */
GLuint graphics_get_shader_program();

/**
 * @brief clear the drawing context and ready the next frame
 */
void graphics_clear_frame();

/**
 * @brief advance the frame, taking into account desired frame rate
 */
void graphics_next_frame();

#endif