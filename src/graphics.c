#include <stdlib.h>
#include <GL\glu.h>
#include "simple_logger.h"
#include "graphics.h"

// "Think of a context as an object that holds all of OpenGL; 
// when a context is destroyed, OpenGL is destroyed."
static SDL_GLContext	__graphics_gl_context;

// window for game
static SDL_Window*		__graphics_window = NULL;

// shader loader
static GLuint			__graphics_shader_program;

// frame delay
static Uint32			__graphics3d_frame_delay = 33;

// private function: close graphics system
void graphics_end();

GLuint graphics_get_shader_program()
{
	return __graphics_shader_program;
}

// private function: enables light
void graphics3d_setup_default_light();

int graphics_init(int sw,int sh,int fullscreen,const char *project, Uint32 frameDelay)
{
	const unsigned char *version; // this must not be changed
	GLenum glew_status;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		slog("SDL could not load\n");
		return -1;
	}
	atexit(SDL_Quit);
}