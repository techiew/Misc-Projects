#pragma once

#include <SDL.h>
#include <GL/glew.h>

class Graphics
{
public:
	SDL_Window* window;

	Graphics(SDL_Window* window);
	void Render();
	~Graphics();
};

