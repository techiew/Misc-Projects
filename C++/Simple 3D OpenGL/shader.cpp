#include "shader.h"

Shader::Shader(const std::string fileName)
{
	shaderProgram = glCreateProgram();
	shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);
	
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(shaderProgram, shaders[i]);
	}

	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "texCoord");

	glLinkProgram(shaderProgram);
	CheckShaderError(shaderProgram, GL_LINK_STATUS, true, "Error, shader program failed to link: ");

	glValidateProgram(shaderProgram);
	CheckShaderError(shaderProgram, GL_VALIDATE_STATUS, true, "Error, shader program is not valid: ");

	uniforms[TRANSFORM_U] = glGetUniformLocation(shaderProgram, "transform");

}

void Shader::Bind()
{
	glUseProgram(shaderProgram);
}

void Shader::Update(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, glm::mat4 view)
{
	glm::mat4 posMatrix = glm::translate(pos);
	glm::mat4 rotationXMatrix = glm::rotate(rotation.x, glm::vec3(1, 0, 0));
	glm::mat4 rotationYMatrix = glm::rotate(rotation.y, glm::vec3(0, 1, 0));
	glm::mat4 rotationZMatrix = glm::rotate(rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 scaleMatrix = glm::scale(scale);

	glm::mat4 rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

	glm::mat4 model = view * (posMatrix * rotationMatrix * scaleMatrix);

	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

GLuint Shader::CreateShader(const std::string & text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		std::cerr << "Error, shader creation failed!" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringsLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringsLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringsLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error, shader compilation failed: ");

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(shaderProgram);
}