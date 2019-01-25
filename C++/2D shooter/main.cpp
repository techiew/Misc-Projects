#include <iostream>
#include <ctime>
#include <vector>

#include "SDL.h"
#include "SDL2_gfxPrimitives.h"

#include "2ds_player.h"
#include "2ds_fpsmanager.h"
#include "2ds_input.h"
#include "2ds_physics.h"
#include "2ds_graphics.h"
#include "2ds_textures.h"
#include "2ds_map.h"
#include "2ds_collision.h"
#include "2ds_shadows.h"
#include "2ds_enemymanager.h"
#include "2ds_rand.h"

//Define global classes
Player* gPlayer;
FPSManager* gFPSManager;
Input* gInput;
Physics* gPhysics;
Graphics* gGraphics;
Textures* gTextures;
Map* gMap;
Collision* gCollision;
Shadows* gShadows;
EnemyManager* gEnemyManager;
Camera2D* gCamera;

int main(int argv, char* argc[]) {
	
	/* ----- Some game options ----- */
	int fps = 60; // Max FPS
	int resX = 1000; // X resolution of window on startup
	int resY = 750; // Y resolution
	int cellHeight = 64; // Cell height used in the grid system
	int cellWidth = 64; // Cell width for the grid system (used in map generation)
	int playerSize = 64; // Size of player (radius in pixels)
	int playerHP = 5; // Health of player
	double movementPerTick = 10; // Movement per frame in pixels for player
	bool renderBounds = false; // Enable or disable rendering of bounding boxes used to test collision detection
	bool isAIEnabled = true; // Enable/disable AI of enemies
	bool showFPSCounter = false; // Toggle FPS counter (shows in console)
	bool isPlayerGod = false; // Toggle godmode
	bool noclip = false; // Toggle walking through walls

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	window = SDL_CreateWindow("2D Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resX, resY, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
	SDL_RenderClear(renderer);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_Surface* surfaceCursor = SDL_LoadBMP("Assets/texture_crosshair.bmp");
	SDL_Cursor* cursor = SDL_CreateColorCursor(surfaceCursor, 20, 20);
	SDL_SetCursor(cursor);

	double delta = 0;
	bool isRunning = true;

	//Set seed used for random number generation
	srand(time(NULL));
	
	//Create map
	gMap = new Map(Vector2(resX / 2, resY / 2), cellHeight, cellWidth);
	gMap->CreateRandomMap();
	gMap->SpawnEnemies();

	//Initialize all parts of the game
	gFPSManager = new FPSManager(fps, showFPSCounter, &delta);
	gPlayer = new Player(playerSize, playerHP, movementPerTick, isPlayerGod);
	gCamera = new Camera2D(resX, resY);
	gCollision = new Collision(renderBounds);
	gInput = new Input(&isRunning, noclip, window);
	gShadows = new Shadows();
	gEnemyManager = new EnemyManager(isAIEnabled);
	gPhysics = new Physics();
	gTextures = new Textures;
	std::map<int, SDL_Texture*> textureList = gTextures->PreLoadTextures(renderer);
	gGraphics = new Graphics(renderer, textureList);

	//Main game loop
	while (isRunning) {
		//Calculations to limit FPS
		gFPSManager->Tick();

		if (gEnemyManager->enemies.size() == 0) {
			gMap->CreateRandomMap();
			gMap->SpawnEnemies();
			gPlayer->OnDeath();
			gEnemyManager = new EnemyManager(isAIEnabled);
			gCollision = new Collision(renderBounds);
		}

		if (delta >= 1) { //Do calculations for the frames that we use
			gInput->Tick();
			gPhysics->Tick();
			gPlayer->Tick();
			gEnemyManager->TickEnemies();
			delta = 0;
		}

		gGraphics->Render(gEnemyManager->enemies.size());
	}

	for (auto const& x : textureList) {
		SDL_DestroyTexture(textureList[x.first]);
	}

	SDL_FreeSurface(surfaceCursor);
	SDL_FreeCursor(cursor);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}