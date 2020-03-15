#include <SDL.h> //core SDL library
#include <SDL2_gfxPrimitives.h> //SDL_gfx graphics primitives library, to draw lines etc.
#include <ctime>
#include <iostream>
#include <vector>
#include "Branch.h"

/* Useful resources:
https://www.youtube.com/watch?v=fcdNSZ9IzJM 
https://github.com/annaflagg/FractalTrees
https://www.rosettacode.org/wiki/Fractal_tree (code for many languages)
*/



void drawBranch(SDL_Renderer* renderer, Branch Branch, int generations) {

	if (Branch.generation == generations) {
		// Add an edge, etc. for direct parents of leaves, makes the Branch get smaller towards the end
		thickLineRGBA(renderer, Branch.start.x, Branch.start.y, Branch.end.x, Branch.end.y, 3, Branch.treec1, Branch.treec2, Branch.treec3, 255);
		thickLineRGBA(renderer, Branch.start.x+2, Branch.start.y, Branch.end.x, Branch.end.y, 3, Branch.treec1, Branch.treec2, Branch.treec3, 255); 
		thickLineRGBA(renderer, Branch.start.x-2, Branch.start.y, Branch.end.x, Branch.end.y, 3, Branch.treec1, Branch.treec2, Branch.treec3, 255);
	}
	else if (Branch.generation > generations) {
		// Leaves are like normal branches, except more transparency
		thickLineRGBA(renderer, Branch.start.x, Branch.start.y, Branch.end.x, Branch.end.y, 3, Branch.leavesc1, Branch.leavesc2, Branch.leavesc3, 150); 
	}
	else {
		// Normal thick line for normal branches
		thickLineRGBA(renderer, Branch.start.x, Branch.start.y, Branch.end.x, Branch.end.y, 6, Branch.treec1, Branch.treec2, Branch.treec3, 255); 
	}

}

// This function creates the tree and returns it in the form of a vector list
std::vector<Branch> createTree(Vector2 origin, int maxBranchLength, int generations, int leafLoops) {
	std::vector<Branch> tree;
	Branch trunk(origin, maxBranchLength); // Start trunk at a given origin and with a maximum length of the branches
	tree.push_back(trunk);
	trunk.numOfBranches = 0;

	for (int g = 0; g < generations+leafLoops; g++) { // Loop for all the generations of branches and leaves
		int vSize = tree.size();

		for (int x = 0; x < vSize; x++) {

			if (tree[x].hasChildren == false) { // If it doesn't have children, then give it children
				tree.push_back(tree[x].createChild(maxBranchLength, true, g, generations)); // Create a left and right Branch
				tree.push_back(tree[x].createChild(maxBranchLength, false, g, generations)); // The branches get pushed into the end of the tree vector list
				tree[x].hasChildren = true; // It is now a parent
			}

		}

	}

	return tree; //Return the finished vector list of all branches + leaves
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;

	srand(time(NULL)); // Give the random function a seed which is the current time

	int resX = 1500, resY = 1100;

	window = SDL_CreateWindow("Fractal_tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Vector2 origin(resX / 2, resY); // Set the origin to be the bottom center of the screen
	int maxBranchLength = 200; // In pixels
	int generations = 5; // Generations of branches
	int leafLoops = 6; // Generations of leaves

	// It's possible to put multiple trees on screen at once, 
	// copy the line below and call the vector something else
	// and make a for loop to render it like in the loop below
	std::vector<Branch> tree = createTree(origin, maxBranchLength, generations, leafLoops); 

	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);

	// Loop over all the branches and draw them
	for (int i = 0; i < tree.size(); i++) {
		drawBranch(renderer, tree[i], generations);
	}

	SDL_RenderPresent(renderer);

	while (true) {

		// Check for events, mouse clicks etc.
		if (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				exit(0);
			}

			// If the mouse is clicked in the window, generate a new tree
			if (event.type == SDL_MOUSEBUTTONDOWN) {

				if (event.button.button == SDL_BUTTON_LEFT) {

					SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
					SDL_RenderClear(renderer);

					tree = createTree(origin, maxBranchLength, generations, leafLoops);

					for (int i = 0; i < tree.size(); i++) {
						drawBranch(renderer, tree[i], generations); 
					}

					SDL_RenderPresent(renderer);

				}

			}

		}

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}