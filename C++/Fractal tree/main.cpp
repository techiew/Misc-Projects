#include <SDL.h> //core SDL library
#include <SDL2_gfxPrimitives.h> //SDL_gfx graphics primitives library, to draw lines etc.
#include <ctime>
#include <iostream>
#include <vector>
#include "branch.h"

/* A little note:
The tree is generated layer by layer, in other words from the bottom, going up
if instead the branches were generated from left to right (using recursion or something), 
the branches would be able to render on top of leaves which looks nice because it gives depth.
As seen here: (look at the yellow tree)
http://www.annaflagg.com/assets/fractaltrees1.png
Code for the picture above (python):
https://github.com/annaflagg/FractalTrees/blob/master/fractaltree.py

Other sources:
https://www.rosettacode.org/wiki/Fractal_tree (code for many languages)
https://www.youtube.com/watch?v=fcdNSZ9IzJM
*/



void drawBranch(SDL_Renderer* renderer, branch branch, int generations) {

	//simpler way of drawing the branch, much better performance.
	//aalineRGBA(renderer, branch.start.x, branch.start.y, branch.end.x, branch.end.y, branch.treec1, branch.treec2, branch.treec3, 255);

	//Better looking branches
	if (branch.generation == generations) {
		//Add an edge, etc. for direct parents of leaves, makes the branch get smaller towards the end
		thickLineRGBA(renderer, branch.start.x, branch.start.y, branch.end.x, branch.end.y, 3, branch.treec1, branch.treec2, branch.treec3, 255);
		thickLineRGBA(renderer, branch.start.x+2, branch.start.y, branch.end.x, branch.end.y, 3, branch.treec1, branch.treec2, branch.treec3, 255); 
		thickLineRGBA(renderer, branch.start.x-2, branch.start.y, branch.end.x, branch.end.y, 3, branch.treec1, branch.treec2, branch.treec3, 255);
	}
	else if (branch.generation > generations) {
		//Leaves are like normal branches, except more transparency
		thickLineRGBA(renderer, branch.start.x, branch.start.y, branch.end.x, branch.end.y, 3, branch.leavesc1, branch.leavesc2, branch.leavesc3, 150); 
	}
	else {
		//Normal thick line for normal branches
		thickLineRGBA(renderer, branch.start.x, branch.start.y, branch.end.x, branch.end.y, 6, branch.treec1, branch.treec2, branch.treec3, 255); 
	}
}

//This function creates the tree and returns it in the form of a vector list
std::vector<branch> createTree(vector2 origin, int maxBranchLength, int generations, int leafLoops) {
	std::vector<branch> tree;
	branch trunk(origin, maxBranchLength); //Start trunk at a given origin and with a maximum length of the branches
	tree.push_back(trunk);
	trunk.numOfBranches = 0;
	for (int g = 0; g < generations+leafLoops; g++) { //Loop for all the generations of branches and leaves
		int vSize = tree.size();
		for (int x = 0; x < vSize; x++) {
			if (tree[x].hasChildren == false) { //If it doesn't have children, then give it children
				tree.push_back(tree[x].createChild(maxBranchLength, true, g, generations)); //Create a left and right branch
				tree.push_back(tree[x].createChild(maxBranchLength, false, g, generations)); //The branches get "pushed" into the end of the tree vector list
				tree[x].hasChildren = true; //It is now a parent
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

	srand(time(NULL)); //Give the random function a seed which is the current time

	int resX = 1500, resY = 1100;

	window = SDL_CreateWindow("Fractal_tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	vector2 origin(resX / 2, resY); //Set the origin to be the bottom center of the screen
	int maxBranchLength = 200; // in pixels
	int generations = 5; //generations of branches
	int leafLoops = 6; //generations of leaves

	//It's possible to put multiple trees on screen at once, 
	//copy the line below and call it something else
	//and make a for loop to render it like in the loop below
	std::vector<branch> tree = createTree(origin, maxBranchLength, generations, leafLoops); 

	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);

	//Loop over all the branches and draw them
	for (int i = 0; i < tree.size(); i++) {
		drawBranch(renderer, tree[i], generations);
	}

	SDL_RenderPresent(renderer);

	while (true) {

		//Check for events, mouse clicks etc.
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}

			//If the mouse is clicked in the window, generate a new tree
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