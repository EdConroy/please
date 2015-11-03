#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>


#define NUM_LIGHTS 3
static float g_lightPosition[NUM_LIGHTS * 3];
static float g_lightColor[NUM_LIGHTS * 3];
static float g_lightRotation;

void shad_attach(GLuint program, GLenum type, const char *filepath);
static GLuint shad_compile(GLenum type, const char *filePath);
static char* shad_load_src(const char* filePath);

static GLuint g_program;
static GLuint g_programLightPositionLocation;
static GLuint g_programLightColorLocation;

#endif