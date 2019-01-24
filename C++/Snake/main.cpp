#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include <SDL.h> //Main SDL library
#include <SDL_ttf.h> //SDL library used to display text with the renderer

/*
Note: this code is now very old at the time that I'm writing this comment and
is probably a big, confusing mess, but it might be useful for someone.
This was one of my first C++ programs
*/

using namespace std;

void renderPlayer(SDL_Renderer* renderer, SDL_Rect player, int x, int y, int scale, vector<int> tailX, vector<int> tailY, int tailLength) {
	//Setting color before rendering, needs to be set first each time or the block could get a color from another block
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	player.w = scale;
	player.h = scale;

	//Gets x and y of all tail blocks and renders them
	for (int i = 0; i < tailLength; i++) {
		player.x = tailX[i];
		player.y = tailY[i];
		SDL_RenderFillRect(renderer, &player);
	}

	player.x = x;
	player.y = y;

	SDL_RenderFillRect(renderer, &player);
}

void renderFood(SDL_Renderer* renderer, SDL_Rect food) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &food);
}

void renderScore(SDL_Renderer* renderer, int tailLength, int scale, int wScale) {
	SDL_Color Black = { 0, 0, 0 };
	//Get the font used for displaying text
	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		//font not loaded? print an error and return
		cout << "Font loading error" << endl;
		return;
	}
	//c_str() allows normal strings to be rendered here, otherwise the rendered text would have to be a const char* type
	//Const char* values cannot be changed after initialization, but we need dynamic text here
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect scoreRect;
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	//Close font before finishing so this font doesn't collide with any other function's font
	TTF_CloseFont(font);
}

bool checkCollision(int foodx, int foody, int playerx, int playery) {
	if (playerx == foodx && playery == foody){
		return true;
	}
	return false;
}

pair <int, int> getFoodSpawn(vector <int> tailX, vector <int> tailY, int playerX, int playerY, int scale, int wScale, int tailLength) {
	//Get a valid spawn for the food which is not on top of a tail or player block
	bool valid = false;
	int x = 0;
	int y = 0;
	srand(time(0));
	x = scale*(rand() % wScale);
	y = scale*(rand() % wScale);
	valid = true;

	//Check all tail blocks and player block
	for (int i = 0; i < tailLength; i++) {
		if ((x == tailX[i] && y == tailY[i]) || (x == playerX && y == playerY)) {
			valid = false;
		}
	}

	if (!valid) {
		pair <int, int> foodLoc;
		foodLoc = make_pair(-100, -100);
		return foodLoc;
	}

	//Create and return a pair of the two position values
	pair <int, int> foodLoc;
	foodLoc = make_pair(x, y);

	return foodLoc;
}

void gameOver(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };

	//Get the font used for displaying text
	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		//font not loaded? print an error and return
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "Game Over", Red);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to retry", White);

	/* 
	c_str() allows normal strings to be rendered here, otherwise the rendered text would have to be a const char* type
	Const char* values cannot be changed after initialization, but this text needs to be able to change
	We still render the score in the game over screen so the player can still see their score 
	*/
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale*wScale) / 2)-(gameoverRect.w/2);
	gameoverRect.y = ((scale*wScale) / 2)-(gameoverRect.h/2)-50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2))+150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	//Close font before finishing so this font doesn't collide with any other function's font
	TTF_CloseFont(font);

	//Show game over screen while space has not been pressed
	while (true) {
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
		}
	}
}

void YouWin(SDL_Renderer* renderer, SDL_Event event, int scale, int wScale, int tailLength) {
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color White = { 255, 255, 255 };
	SDL_Color Black = { 0, 0, 0 };
	SDL_Color Yellow = { 255, 255, 0 };

	//Get the font used for displaying text
	TTF_Font* font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		//font not loaded? print an error and return
		cout << "Font loading error" << endl;
		return;
	}

	SDL_Surface* gameover = TTF_RenderText_Solid(font, "You won!", Yellow);
	SDL_Surface* retry = TTF_RenderText_Solid(font, "Press Enter to play again", White);

	/*
	c_str() allows normal strings to be rendered here, otherwise the rendered text would have to be a const char* type
	Const char* values cannot be changed after initialization, but this text needs to be able to change
	We still render the score in the game over screen so the player can still see their score
	*/
	SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(tailLength * 10)).c_str(), Black);
	SDL_Texture* gameoverMessage = SDL_CreateTextureFromSurface(renderer, gameover);
	SDL_Texture* retryMessage = SDL_CreateTextureFromSurface(renderer, retry);
	SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(renderer, score);
	SDL_Rect gameoverRect;
	SDL_Rect retryRect;
	SDL_Rect scoreRect;
	gameoverRect.w = 200;
	gameoverRect.h = 100;
	gameoverRect.x = ((scale*wScale) / 2) - (gameoverRect.w / 2);
	gameoverRect.y = ((scale*wScale) / 2) - (gameoverRect.h / 2) - 50;
	retryRect.w = 300;
	retryRect.h = 50;
	retryRect.x = ((scale*wScale) / 2) - ((retryRect.w / 2));
	retryRect.y = (((scale*wScale) / 2) - ((retryRect.h / 2)) + 150);
	scoreRect.w = 100;
	scoreRect.h = 30;
	scoreRect.x = ((scale*wScale) / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
	SDL_RenderCopy(renderer, gameoverMessage, NULL, &gameoverRect);
	SDL_RenderCopy(renderer, retryMessage, NULL, &retryRect);
	SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);
	//Close font before finishing so this font doesn't collide with any other function's font
	TTF_CloseFont(font);

	//Show victory screen while space has not been pressed
	while (true) {
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				return;
			}
		}
	}
}

int main(int argc, char* argv[]) {

	//Init everything so we have everything
	SDL_Init(SDL_INIT_EVERYTHING);

	//Init TTF and check for any errors
	if (TTF_Init() < 0) {
		cout << "Error: " << TTF_GetError() << endl;
	}

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	//This is the player rectangle, set all values to 0
	SDL_Rect player;
	player.x = 0;
	player.y = 0;
	player.h = 0;
	player.w = 0;

	//tailLength is incremented every time the snake eats food
	//Used in for loops to determine how many times they have to run
	int tailLength = 0;

	//Vectors for storage of tail block positions
	//Vectors are used because they don't need to have a fixed size, unlike normal arrays
	vector<int> tailX;
	vector<int> tailY;

	//These values change scale of blocks and scale of window (wScale)
	//They are later multiplied with eachother to allow easy change of window size according to block size
	int scale = 24;
	int wScale = 24;

	//Default values of player position values
	int x = 0;
	int y = 0;
	int prevX = 0;
	int prevY = 0;

	//Booleans to control the movement
	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;

	bool inputThisFrame = false;
	bool redo = false;

	//This is the food rectangle, set all values to 0
	//Except for the x and y coordinates, which will be set to something random inside the window area
	SDL_Rect food;
	food.w = scale;
	food.h = scale;
	food.x = 0;
	food.y = 0;
	
	pair <int, int> foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
	food.x = foodLoc.first;
	food.y = foodLoc.second;

	//Show the window with these settings and apply a renderer to it
	//The renderer is responsible for all graphics being displayed
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scale*wScale+1, scale*wScale+1, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Used for delaying movement of blocks
	float time = SDL_GetTicks() / 100;

	while (true) { //Main game loop, this constantly runs and keeps everything updated

		//Used to delay the movement of blocks
		float newTime = SDL_GetTicks() / 75; //This value (75) is the speed at which the blocks are updated
		float delta = newTime - time;
		time = newTime;

		inputThisFrame = false;

		if (tailLength >= 575) {
			YouWin(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
			food.x = foodLoc.first;
			food.y = foodLoc.second;
		}

		//Controls
		if (SDL_PollEvent(&event)) {
			//If user tries to exit window, exit program
			if (event.type == SDL_QUIT) {
				exit(0);
			}
			//If a key is pressed
			if (event.type == SDL_KEYDOWN && inputThisFrame == false) {
				//Then check for the key being pressed and change direction accordingly
				if (down == false && event.key.keysym.scancode == SDL_SCANCODE_UP) {
					up = true;
					left = false;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (right == false && event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					up = false;
					left = true;
					right = false;
					down = false;
					inputThisFrame = true;
				}
				else if (up == false && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
					up = false;
					left = false;
					right = false;
					down = true;
					inputThisFrame = true;
				}
				else if (left == false && event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					up = false;
					left = false;
					right = true;
					down = false;
					inputThisFrame = true;
				}
			}
		}

		//The previous position of the player block, these are used so tail blocks will never be on the player block's position
		prevX = x;
		prevY = y;

		//delta*xMovementPerTick
		//This makes the player move at the same speed regardless of framerate
		//Without this the player would move way too fast
		if (up) {
			y -= delta*scale;
		}
		else if (left) {
			x -= delta*scale;
		}
		else if (right) {
			x += delta*scale;
		}
		else if (down) {
			y += delta*scale;
		}

		if (redo == true) {
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
		}

		//Collision detection, has played collided with food?
		if (checkCollision(food.x, food.y, x, y)) {
			//Spawn new food after it has been eaten
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
			tailLength++;
		}

		//Only runs in the frames where the player block has moved
		if (delta*scale == 24) {
			//Update tail size and position
			//If the size of the tail isn't equal to the actual size, add another block to the tail (this needs to be first)
			//If this is not first the program will later attempt to check and move an amount of tail blocks equal to the tailLength -
			//(which is constantly updated), although the extra tail block hasn't been added, causing a crash
			if (tailX.size() != tailLength) {
				tailX.push_back(prevX);
				tailY.push_back(prevY);
			}

			//Loop through every tail block, move all blocks to the nearest block in front
			//This updates the blocks from end (farthest from player block) to the start (nearest to player block)
			for (int i = 0; i < tailLength; i++) {
				if (i > 0) {
					tailX[i - 1] = tailX[i];
					tailY[i - 1] = tailY[i];
				}
			}

			/*
			If the tail has grown, change the nearest tail block position to the player block position (needs to be last, otherwise, the 2 first blocks stack)
			The tail blocks will stack because the 2nd block will always have an updated position to the first block
			While the third block and the rest have an outdated position to the block infront 
			*/
			if (tailLength > 0) {
				tailX[tailLength - 1] = prevX;
				tailY[tailLength - 1] = prevY;
			}
		}
		
		//Game over if player has collided with a tail block, also reset everything
		for (int i = 0; i < tailLength; i++) {
			if (x == tailX[i] && y == tailY[i]) {
				gameOver(renderer, event, scale, wScale, tailLength);
				x = 0;
				y = 0;
				up = false;
				left = false;
				right = false;
				down = false;
				tailX.clear();
				tailY.clear();
				tailLength = 0;
				redo = false;
				foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
				if (food.x == -100 && food.y == -100) {
					redo = true;
				}
				food.x = foodLoc.first;
				food.y = foodLoc.second;
			}
		}

		//Game over if player out of bounds, also reset everything
		if (x < 0 || y < 0 || x > scale*wScale-scale || y > scale*wScale-scale) {
			gameOver(renderer, event, scale, wScale, tailLength);
			x = 0;
			y = 0;
			up = false;
			left = false;
			right = false;
			down = false;
			tailX.clear();
			tailY.clear();
			tailLength = 0;
			redo = false;
			foodLoc = getFoodSpawn(tailX, tailY, x, y, scale, wScale, tailLength);
			food.x = foodLoc.first;
			food.y = foodLoc.second;
			if (food.x == -100 && food.y == -100) {
				redo = true;
			}
		}

		//Render everything
		renderFood(renderer, food);
		renderPlayer(renderer, player, x, y, scale, tailX, tailY, tailLength);
		renderScore(renderer, tailLength, scale, wScale);

		SDL_RenderDrawLine(renderer, 0, 0, 0, 24 * 24);
		SDL_RenderDrawLine(renderer, 0, 24*24, 24 * 24, 24 * 24);
		SDL_RenderDrawLine(renderer, 24*24, 24 * 24, 24*24, 0);
		SDL_RenderDrawLine(renderer, 24*24, 0, 0, 0);

		//Put everything on screen
		//Nothing is actually put on screen until this is called
		SDL_RenderPresent(renderer);

		//Choose a color and fill the entire window with it, this resets everything before the next frame
		//This also give us our background color
		SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255);
		SDL_RenderClear(renderer);
	}

	SDL_DestroyWindow(window);

	TTF_Quit();

	SDL_Quit();

	return 0;

}