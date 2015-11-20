#include <stdio.h>
#include "graphics3.h"
#include "simple_logger.h"

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

int graphx_init()
{
	const unsigned char* renderer;
	const unsigned char* version;

	slog("attempt to start glfw %s instead of SDL\n", glfwGetVersionString());
	
	glfwSetErrorCallback(graphx_glfw_errors);

	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: YOU FAIL HARD AF");
		return -1;
	}

	game_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "mY mIxTaPe", NULL, NULL);

	if (!game_window)
	{
		fprintf (stderr, "ERROR: SO CLOSE! YOU STILL FAIL HARD AF");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(game_window);

	glfwWindowHint(GLFW_SAMPLES, 16); // <<< DAFUQ

	glewExperimental = GL_TRUE;
	glewInit();

	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);

	printf("RENDERER: %s\n", renderer);
	printf("OPENGL VERSION SUPPORTED: %s\n", version);
	
	slog("renderer: %s\n version: %s\n", renderer, version);

	return 0;
}

void graphx_update()
{
	glfwSwapBuffers(game_window);
}

pbool graphx_windowOpen()
{
	while (!glfwWindowShouldClose(game_window))
		 return true;
	return false;
}

GLFWwindow* getGameWindow()
{
	return game_window;
}