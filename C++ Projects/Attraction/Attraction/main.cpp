#include <SDL.h> 
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include <vector>
#include "particle.h" 

/*
* This program is based on this tutorial by The Coding Train:
* https://www.youtube.com/watch?v=OAcXnzRNiCY
* 
* The graphics were inspired by Muzkaw's video:
* https://www.youtube.com/watch?v=RMfsdUKEmDo
*/

double map(int number, int fromMin, int fromMax, int toMin, int toMax)
{
	return (number - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

// Draws each particle
void DrawParticle(SDL_Renderer* renderer, Particle par) { 
	int colorIntensity1 = std::abs(par.vel.first);
	int colorIntensity2 = std::abs(par.vel.second);
	int color = map(colorIntensity1 + colorIntensity2, 0, 1, 0, 255);
	filledCircleRGBA(renderer, par.pos.first, par.pos.second, 1, color, 0, color, 255);
}

std::vector<Particle> SpawnParticles(std::pair<int, int> origin) {
	std::vector<Particle> particles;

	int numParticles = 11000;
	int row = 0;
	int column = 0;

	for (int i = 0; i < numParticles; i++) {

		if (i % 120 == 0) {
			row++;
			column = 0;
		}

		Particle par(origin.first + (column * 10), origin.second + (row * 10));
		particles.push_back(par);
		column++;

	}

	return particles;
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;

	// Resolution of the window
	int resX = 1200;
	int resY = 900;

	std::vector<Particle> particles = SpawnParticles(std::make_pair(110, resY / 4));

	window = SDL_CreateWindow("Attract", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	bool mousePressed = false;

	int mouseX = 0;
	int mouseY = 0;
	std::pair<int, int> mousePos = std::make_pair(0, 0);

	while (true) {

		SDL_GetMouseState(&mouseX, &mouseY);
		mousePos = std::make_pair(mouseX, mouseY);

		// Attract particles and draw them
		for (int i = 0; i < particles.size(); i++) {

			if (mousePressed) {
				particles[i].Attract(mousePos);
			}

			particles[i].Update(resX, resY);
			DrawParticle(renderer, particles[i]);
		}

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Check for inputs, mouse presses, etc
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			if (event.type == SDL_MOUSEBUTTONDOWN) {

				if (event.button.button == SDL_BUTTON_LEFT) {
					mousePressed = true;
				}

				if (event.button.button == SDL_BUTTON_RIGHT) { // Move all particles to the cursor
					particles.clear();
					particles = SpawnParticles(std::make_pair(5, -5));;
				}

			} else if (event.type == SDL_MOUSEBUTTONUP) {

				if (event.button.button == SDL_BUTTON_LEFT) {
					mousePressed = false;
				}

			}

		}

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}