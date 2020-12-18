#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include "graphics.h"
#include "shader.h"
#include "mesh.h"
#include "vertex.h"
#include "texture.h"

int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	int resX = 800;
	int resY = 600;
	bool isRunning = true;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("3D OpenGL Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	GLenum res = glewInit();

	if (res != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize!" << std::endl;
	}

	glViewport(0, 0, resX, resY);

	SDL_Event e;

	Graphics graphics(window);

	// Defining the triangle model
	Vertex vertices[] =
	{
		//Bottom
		Vertex(glm::vec3(0, 0, 1), glm::vec4(255, 0, 0, 255)),
		Vertex(glm::vec3(0.5, 0, 0), glm::vec4(255, 0, 0, 255)),
		Vertex(glm::vec3(-0.5, 0, 0), glm::vec4(255, 0, 0, 255)),

		//Front
		Vertex(glm::vec3(0, 1, 0.5), glm::vec4(0, 255, 0, 255)),
		Vertex(glm::vec3(-0.5, 0, 0), glm::vec4(0, 255, 0, 255)),
		Vertex(glm::vec3(0.5, 0, 0), glm::vec4(0, 255, 0, 255)),

		//Right
		Vertex(glm::vec3(0, 1, 0.5), glm::vec4(0, 0, 255, 255)),
		Vertex(glm::vec3(0.5, 0, 0), glm::vec4(0, 0, 255, 255)),
		Vertex(glm::vec3(0, 0, 1), glm::vec4(0, 0, 255, 255)),

		//Left
		Vertex(glm::vec3(0, 1, 0.5), glm::vec4(255, 0, 255, 255)),
		Vertex(glm::vec3(0, 0, 1), glm::vec4(255, 0, 255, 255)),
		Vertex(glm::vec3(-0.5, 0, 0), glm::vec4(255, 0, 255, 255)),
	};

	Texture texture;
	texture.Bind(0);

	Shader shader(".\\res\\testshader");
	shader.Bind();

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]));

	glm::vec3 pos = glm::vec3();
	glm::vec3 rotation = glm::vec3();
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	float counter = 0;

	// Setting up the camera
	glm::vec3 cameraPos = glm::vec3(0, 0, 3);
	float cameraFOV = 70.0f;
	float aspectRatio = (float)resX / (float)resY;
	float zNear = 0.01f;
	float zFar = 1000.0f;
	glm::mat4 perspectiveMatrix = glm::perspective(cameraFOV, aspectRatio, zNear, zFar);
	glm::vec3 forward = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::mat4 view = perspectiveMatrix * glm::lookAt(cameraPos, cameraPos + forward, up);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	while (isRunning)
	{
		pos.x = sinf(counter);
		pos.y = cosf(counter);

		rotation = glm::vec3(counter, 0, counter);
		//scale = glm::vec3(cosf(counter), cosf(counter), cosf(counter));
		view = perspectiveMatrix * glm::lookAt(cameraPos, cameraPos + forward, up);
		shader.Update(pos, rotation, scale, view);

		mesh.Draw();
		graphics.Render();

		counter += 0.01;

		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_QUIT)
			{
				isRunning = false;
			}

			if (e.type == SDL_WINDOWEVENT)
			{

				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					glViewport(0, 0, e.window.data1, e.window.data2);
				}

			}

		}

	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}