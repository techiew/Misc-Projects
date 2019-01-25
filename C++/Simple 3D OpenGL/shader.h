#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm.hpp>
#include <transform.hpp>

class Shader
{
public:
	enum
	{
		TRANSFORM_U,
		TEXCOORD_U,

		NUM_UNIFORMS
	};

	static const unsigned int NUM_SHADERS = 2;
	GLuint shaderProgram;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNIFORMS];

	Shader(const std::string fileName);
	void Bind();
	void Update(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, glm::mat4 view);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);
	std::string LoadShader(const std::string& fileName);
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	~Shader();
};

