#include <stdlib.h>
#include "graphics.h"
#include "shader.h"
#include "simple_logger.h"
#include "text.h"


GLint	view[4];
GLdouble model[16];
GLdouble projection[16];

// extern: text
extern Sprite *ascii;

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
void graphics_setup_default_light();

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

	// OpenGL
	__graphics_gl_context = SDL_GL_CreateContext(__graphics_window);
	if(__graphics_gl_context == NULL)
	{
        slog("There was an error creating the OpenGL context!\n");
        return -1;
	}

	version = glGetString(GL_VERSION);
	if (version == NULL) 
    {
        slog("There was an error creating the OpenGL context!\n");
        return -1;
    }

	// attached OpenGL context to the window
	SDL_GL_MakeCurrent(__graphics_window, __graphics_gl_context);

	// ???
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	glewExperimental = GL_TRUE;
    glew_status = glewInit();
	if (glew_status != 0) 
    {
        slog("Error: %s", glewGetErrorString(glew_status));
        return -1;
    }
	
	glViewport(0,0,sw, sh);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective( 40, (float)sw / (float)sh, .01, 2000.0f);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	glClearColor(0.0,0.0,0.0,0.0);
    glClear( 1 );

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_LESS);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	graphics_setup_default_light();

	// save mvp in variables
	glGetIntegerv(GL_VIEWPORT, view);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	atexit(graphics_end);
	return 0;
}

void graphics_end()
{
	SDL_GL_DeleteContext(__graphics_gl_context);
	__graphics_frame_delay = 0;
}

void graphics_clear_frame()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glPushMatrix();
}

void graphics_next_frame()
{
	static Uint32 then = 0;
	Uint32 now;

	//drawToTheFrigginScreen(vec2d(58, 14), vec2d(0.8f, -1.0f), ascii->texture);

	glPopMatrix();

	SDL_GL_SwapWindow(__graphics_window); // update frames

	now = SDL_GetTicks(); // update time

	then = now;
}

void graphics_setup_default_light()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

	GLfloat light_position[] = { -10.0, -10.0, 10.0, 0.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light1_ambient[] = { 1.2, 1.2, 1.2, 1.0 };
    GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light1_position[] = { -20.0, 2.0, 1.0, 1.0 };
    GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };

	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.9);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.7);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 80.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
}

void windowToGL(GLint x, GLint y, GLdouble z, const GLdouble * model, const GLdouble * proj, const GLint * view, GLdouble * glx, GLdouble * gly, GLdouble * glz)
{
	GLdouble srcx, srcy;
	srcx = (GLdouble)x;

	// windows coordinates start with 0,0 (top left), but OpenGL starts at the lower left
	// which is why we subtract 
	srcy = view[3] - (GLdouble)y;

	// gimme the coordinates, store in glx, gly, glz
	gluUnProject(srcx, srcy, z, model, proj, view, glx, gly, glz);
}