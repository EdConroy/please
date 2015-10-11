#include <stdlib.h>
#include "graphics.h"
#include "simple_logger.h"

// WARNING IN CASE OF UNRESOLVED EXTERNAL SYMBOLS
// IF YOUR .H OR .C FILE WAS ORIGINALLY OF ANOTHER FORMAT,
// CREATE ENTIRE NEW FILE (IT MUST BE .H OR .C TO BEGIN WITH)

// "Think of a context as an object that holds all of OpenGL; 
// when a context is destroyed, OpenGL is destroyed."
static SDL_GLContext	__graphics_gl_context;

// window for game
static SDL_Window*		__graphics_window = NULL;

// shader loader
static GLuint			__graphics_shader_program;

// frame delay
static Uint32			__graphics_frame_delay = 33;

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

	__graphics_frame_delay = frameDelay;

	__graphics_window = SDL_CreateWindow
		(
		project?project:"please",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		sw, sh,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
		);

	atexit(graphics_end);

	return 0;
}

void graphics_end()
{
	__graphics_frame_delay = 0;
}