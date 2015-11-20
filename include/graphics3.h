#ifndef _GRAPHICSX_H_
#define	_GRAPHICSX_H_

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "collisions.h"

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

int graphx_init();

pbool graphx_init_shader();
pbool graphx_init_program(const char* vert, const char* frag);

void graphx_glfw_errors(int error, const char* description);
void graphx_glfw_winsize(GLFWwindow* window, int width, int height);

void graphx_update();
pbool graphx_windowOpen();
GLFWwindow* getGameWindow();

#endif