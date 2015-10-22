#version 330
layout(location = 1) in vec3 vertexColor;
out vec3 fragmentColor;
void main()
{
	fragmentColor = vertexColor;
}