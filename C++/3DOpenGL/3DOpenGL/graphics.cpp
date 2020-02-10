#include "graphics.h"

Graphics::Graphics(SDL_Window* window)
{
	this->window = window;
}


void Graphics::Render()
{
	SDL_GL_SwapWindow(window);
	glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}