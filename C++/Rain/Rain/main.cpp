#include <sdl.h> // Core SDL library
#include <SDL2_gfxPrimitives.h> // SDL2_gfx, graphics library used to draw circles, triangles, etc.
#include <iostream>
#include <vector> 
#include <ctime> 
#include "vector2.h" 

/*
* Welcome to my simple and extremely inefficient liquid simulation
* Made with SDL2 (creates the window, manages input, etc.) and SDL2_gfx (for drawing circles and other shapes) 
* Controls: left click to create obstacles, right click to erase obstacles, press mouse wheel to remove all obstacles
*/


// Get a random number in range from minimum to maximum
int getRand(int rMin, int rMax) { 
	int r = rand() % (rMax - rMin + 1) + rMin;
	return r;
}

// Collision detection (for vectors)
bool isColliding(std::vector<vector2> obstacles, int x1, int y1, int sizeOfObjects) { 

	for (int i = 0; i < obstacles.size(); i++) {
		int x2 = obstacles[i].x;
		int y2 = obstacles[i].y;

		if (x1 < x2 + sizeOfObjects &&
			x1 + sizeOfObjects > x2 &&
			y1 < y2 + sizeOfObjects &&
			sizeOfObjects + y1 > y2) {
			return true;
		}

	}

	return false;
}

// Collision detection
bool isColliding(int x1, int y1, int x2, int y2, int sizeOfObjects) {

	if (x1 < x2 + sizeOfObjects &&
		x1 + sizeOfObjects > x2 &&
		y1 < y2 + sizeOfObjects &&
		sizeOfObjects + y1 > y2) {
		return true;
	}

	return false;
}

// Draw the raindrops
void drawDrop(SDL_Renderer* renderer, int x, int y, int sizeOfObjects) {
	filledCircleRGBA(renderer, x, y, sizeOfObjects, 0, 0, 255, 150);
}

// Draw the obstacles
void drawObstacle(SDL_Renderer* renderer, int x, int y, int sizeOfObjects) {
	filledCircleRGBA(renderer, x, y, sizeOfObjects, 50, 0, 0, 255);
}

// Draw a circle on the cursor, rendered while erasing
void drawCursor(SDL_Renderer* renderer, int x, int y) {
	filledCircleRGBA(renderer, x, y, 10, 255, 255, 255, 255);
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	bool isRunning = true;
	int resX = 1200;
	int resY = 900;

	srand(time(NULL));

	window = SDL_CreateWindow("Rain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	std::vector<vector2> drops;
	std::vector<vector2> otherDrops;
	std::vector<vector2> obstacles;
	vector2 prevObstacle(0, 0);

	bool isMousePressed = false;
	bool isErasing = false;
	int sizeOfObjects = 15;
	int speedOfDrops = 20;
	int frameCounter = 0;
	int mouseX = 0, mouseY = 0;

	double currentTime = SDL_GetTicks();
	double lastTime = SDL_GetTicks();
	double timePerFrame = 1000 / 60;

	while (isRunning) {
		// This is my 'attempt' at creating some sort of timestep,
		// I'm pretty sure it doesn't actually work, so ignore it.
		// --
		currentTime = SDL_GetTicks();

		if (currentTime - lastTime < timePerFrame) continue;

		lastTime += timePerFrame;
		// --

		frameCounter++;

		if (drops.size() < 1000) {
			drops.push_back(vector2(getRand(0, resX), 0));
			frameCounter = 0;
		}

		for (int i = 0; i < drops.size(); i++) {

			otherDrops = drops; // otherDrops is used to get all drops excluding the current one
			otherDrops.erase(otherDrops.begin() + i);

			// Check if a drop is stuck in an obstacle, if so, move it upwards.
			if (isColliding(obstacles, drops[i].x, drops[i].y, sizeOfObjects)) {
				drops[i].set(drops[i].x, drops[i].y - sizeOfObjects);
			}

			// Check if a drop is colliding with an obstacle or drop below it, if not, move down
			if (!isColliding(obstacles, drops[i].x, drops[i].y + sizeOfObjects, sizeOfObjects)
				&& !isColliding(otherDrops, drops[i].x, drops[i].y + sizeOfObjects, sizeOfObjects)) {
				drops[i].set(drops[i].x, drops[i].y + speedOfDrops);

				// Check if a drop is colliding with an obstacle or drop to the left, if not, move left
			} else if (!isColliding(obstacles, drops[i].x - sizeOfObjects, drops[i].y, sizeOfObjects)
				&& !isColliding(otherDrops, drops[i].x - sizeOfObjects, drops[i].y, sizeOfObjects)) {
				drops[i].set(drops[i].x - (speedOfDrops / 4), drops[i].y);

				// Check if a drop is colliding with an obstacle or drop to the right, if not, move right
			} else if (!isColliding(obstacles, drops[i].x + sizeOfObjects, drops[i].y, sizeOfObjects)
				&& !isColliding(otherDrops, drops[i].x + sizeOfObjects, drops[i].y, sizeOfObjects)) {
				drops[i].set(drops[i].x + (speedOfDrops / 4), drops[i].y);
			}

			drawDrop(renderer, drops[i].x, drops[i].y, sizeOfObjects);

			// If a drop is outside the window, remove it
			if (drops[i].x > resX || drops[i].x < 0 || drops[i].y > resY || drops[i].y < 0) {
				drops.erase(drops.begin() + i);
			}

		}

		//Draw all obstacles
		for (int i = 0; i < obstacles.size(); i++) {
			drawObstacle(renderer, obstacles[i].x, obstacles[i].y, sizeOfObjects);
		}

		//Erase when right clicking
		if (isErasing) {
			SDL_GetMouseState(&mouseX, &mouseY);
			drawCursor(renderer, mouseX, mouseY);

			for (int i = 0; i < obstacles.size(); i++) {

				//Check if the cursor is colliding with any obstacles and remove them
				if (isColliding(obstacles[i].x, obstacles[i].y, mouseX, mouseY, sizeOfObjects)) {
					obstacles.erase(obstacles.begin() + i);
				}

			}

		}

		//Check for events, mouse clicks etc.
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				isRunning = false;
			}

			if (event.type == SDL_MOUSEBUTTONDOWN) {

				if (event.button.button == SDL_BUTTON_LEFT) {
					isMousePressed = true;
					obstacles.push_back(vector2(event.button.x, event.button.y));
					prevObstacle.set(event.button.x, event.button.y);
				}

				if (event.button.button == SDL_BUTTON_MIDDLE) {
					obstacles.clear();
				}

				if (event.button.button == SDL_BUTTON_RIGHT) {
					isErasing = true;
				}

			}

			if (event.type == SDL_MOUSEBUTTONUP) {

				if (event.button.button == SDL_BUTTON_LEFT) {
					isMousePressed = false;
				}

				if (event.button.button == SDL_BUTTON_RIGHT) {
					isErasing = false;
				}

			}

			if (event.type == SDL_MOUSEMOTION) {

				if (isMousePressed) {
					// If the mouse is not colliding with the previously created obstacle, create another obstacle
					if (!isColliding(event.button.x, event.button.y, prevObstacle.x, prevObstacle.y, sizeOfObjects)) {
						obstacles.push_back(vector2(event.button.x, event.button.y));
						prevObstacle.set(event.button.x, event.button.y);
					}

				}

			}

		}

		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 51, 51, 51, 255);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}