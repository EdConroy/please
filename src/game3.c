#include <glib.h>
#include "graphics3.h"
#include "simple_logger.h"
#include "game.h"
#include "entity.h"

GameData	game;

Entity*		player;

GList		*it;

static double previous_seconds;
double current_seconds;
double elapsed_seconds;

GLuint game_depth_fb;
GLuint game_depth_fb_tex;

GLuint vao;
GLuint vbo;

GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

// extern: game
extern int	game_running;

// extern: entity
extern Entity* __entity_list;

// private declarations: game
void		game_Poll();		// glfwPollEvents ();
void		game_Update();
void		game_Draw();

int		Run();

// defining what happens for player input
void game_Poll()
{
	glfwPollEvents ();

	// will look for input here, still gotta do that
	if (GLFW_PRESS == glfwGetKey(getGameWindow(), GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(getGameWindow(), 1);
}

void game_Update()
{
	// nothing in here yet excpet general stuff
	previous_seconds = glfwGetTime ();
	current_seconds = glfwGetTime ();
	elapsed_seconds = current_seconds - previous_seconds;
	previous_seconds = current_seconds;
}

void game_Draw()
{
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glClearColor(0.2, 0.2, 0.2, 1.0);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glGenTextures(1, &game_depth_fb_tex);
	//glActiveTexture (GL_TEXTURE0);
	//glBindTexture (GL_TEXTURE_2D, game_depth_fb_tex);

	//glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	
	graphx_update();
}

int Run()
{
	game_Poll();
	game_Update();
	game_Draw();
	
	return 1;
}

int game_Init()
{
	game.Run = Run;

	init_logger("please_log.log");	// log for errors and such
	if (graphx_init() != 0)
	{
		slog("FUCKED UP ON DA NEW GRAPHIX THERE BUCKO");
		return -1;
	}

	//const char* vertex_shader =

	//VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof (GLfloat), points, GL_STATIC_DRAW);

	//VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	slog("we made it");
	return 1;
}

void game_Exit()
{
	// deInit everything here
	game.Run = NULL;
	glfwTerminate();
}