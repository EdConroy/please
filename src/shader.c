#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "simple_logger.h"
#include "shader.h"

// called in < game_Init() >
void shad_attach(GLuint program, GLenum type, const char *filepath)
{
	/* step 1: compile */
	GLuint shader = shad_compile(type, filepath);
	
	/* step ?: check the shader, if exists attach to program */
	if (shader != 0)
	{
		glAttachShader(program, shader);

		// actually won't be destroyed until program is destroyed
		glDeleteShader(shader);
	}
}

// called in < shad_attach() >
static GLuint shad_compile(GLenum type, const char *filePath)
{
	char *source;
	GLchar log[4096];
	char strShaderType[16];
	GLuint shader;
	GLint length, result;

	/* step 2: get the shader source (whatever that means), and check fro null */
	source = shad_load_src(filePath);

	if (!source) return 0;

	/* step ?: create shader obj, set the source, and compile */
	shader = glCreateShader(type);
	length = strlen(source);
	glShaderSource(shader, 1, (const char **) &source, &length); // ???
	glCompileShader(shader);
	free(source);

	/* check fro failure */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		glGetShaderInfoLog(shader, length, NULL, log);

		switch(type)
        {
            case GL_VERTEX_SHADER:
                sprintf(strShaderType, "vertex");
                break;
            case GL_GEOMETRY_SHADER:
                sprintf(strShaderType, "geometry");
                break;
            case GL_FRAGMENT_SHADER:
                sprintf(strShaderType, "fragment");
                break;
        }

		slog ("FAILURE in %s shader: \n%s", strShaderType, log);
		return 0;
	}
}

// called in < shad_compile() >
static char* shad_load_src(const char* filePath)
{
	// take entire file, and put it into string
	const size_t blockSize = 512;
	FILE *fp;
	char buf [512];
	char *source = NULL;
	size_t tmp, sourceLength = 0;

	/* open file */
	fp = fopen(filePath, "r");

	if (!fp) 
	{
		slog("shad_load_src: can't open %s for reading\n", filePath);
		return NULL;
	}

	/* read entire file into a string */
	while((tmp = fread(buf, 1, blockSize, fp)) > 0 )
	{
		char *newSource = malloc(sourceLength + tmp + 1); // make enough space
		if (!newSource)
		{
			slog("MALLOC FILE FAILURE\n");

			// not gonna need this if i failed to load file!
			if (source) 
				free (source);

			return NULL;
		}

		if (source)
		{
			// using buffer to populate newSource
			memcpy(newSource, source, sourceLength);
			free(source);
		}

		memcpy(newSource + sourceLength, buf, tmp);
		source = newSource;
		sourceLength += tmp;
	}

	/* close the file, null string, source should be full */
	fclose(fp);
	if (source)
		source[sourceLength] = '\0';

	return source;
}