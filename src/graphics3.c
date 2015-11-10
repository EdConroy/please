#include <stdio.h>
#include "graphics3.h"
#include "simple_logger.h"

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

static GLFWwindow*	game_window = NULL;

void graphx_glfw_errors(int error, const char* description)
{
	fputs (description, stderr);
	slog("%s\n", description);
}

void graphx_glfw_winsize(GLFWwindow* window, int width, int height)
{
	//
}

pbool graphx_init()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	slog("attempt to start glfw %s instead of SDL\n", glfwGetVersionString());
	
	glfwSetErrorCallback(graphx_glfw_errors);

	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: YOU FAIL HARD AF");
		return false;
	}

	game_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "mY mIxTaPe", NULL, NULL);

	if (!game_window)
	{
		fprintf (stderr, "ERROR: SO CLOSE! YOU STILL FAIL HARD AF");
		glfwTerminate();
		return false;
	}

	// why dafuq is there is window size callback?????

	glfwMakeContextCurrent(game_window);

	glfwWindowHint(GLFW_SAMPLES, 16); // <<< DAFUQ

	glewExperimental = GL_TRUE;
	glewInit();

	printf("RENDERER: %s\n", renderer);
	printf("OPENGL VERSION SUPPORTED: %s\n", version);
	
	slog("renderer: %s\n version: %s\n", renderer, version);

	return true;
}