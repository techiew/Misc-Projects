#pragma once

#include "vertex.h"
#include <GL/glew.h>
#include <vector>

class Mesh
{
public:

	enum
	{
		POSITION_VB,
		COLOR_VB,

		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
	unsigned int drawCount;

	Mesh(Vertex* vertices, unsigned int numVertices);
	void Draw();
	~Mesh();
};

