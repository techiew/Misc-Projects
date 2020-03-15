#include <iostream>
#include <ctime>
#include <vector>
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "player.h"
#include "fpsmanager.h"
#include "input.h"
#include "physics.h"
#include "graphics.h"
#include "textures.h"
#include "map.h"
#include "collision.h"
#include "enemymanager.h"
#include "rand.h"

int main(int argv, char* argc[]) {
	
	/* ----- Some game settings ----- */
	int fps = 60; // Max FPS
	int resX = 1000; // X resolution of the game window
	int resY = 750; // Y resolution of the game window
	int tileHeight = 64; // Height of tiles in the map
	int tileWidth = 64; // Width of tiles in the map
	int playerSize = 64; // Radius of the player circle
	int playerHP = 5; // Player starting health
	double movementPerTick = 10; // Movement per frame in pixels for player
	bool renderBounds = false; // Enable or disable rendering of bounding boxes used to test collision detection
	bool isAIEnabled = true;  // Whether enemy AI is enabled or not
	bool showFPSCounter = false; // Shows in console
	bool isPlayerGod = false; // Toggle godmode
	bool noclip = false; // Toggle walking through walls

	// Set up the game window using SDL
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

	// Sets seed used for random number generation
	srand(time(NULL));
	
	// Create map
	Map map = Map(Vector2(resX / 2, resY / 2), tileHeight, tileWidth);
	map.CreateRandomMap();
	map.SpawnEnemies();

	// Initialize all game objects
	FPSManager fpsManager = FPSManager(fps, showFPSCounter, &delta);
	Camera2D camera = Camera2D(resX, resY);
	Player player = Player(&map, &camera, playerSize, playerHP, movementPerTick, isPlayerGod);
	camera.SetPlayer(&player);
	Collision collision = Collision(&player, &map, renderBounds);
	Input input = Input(&player, &camera, &collision, &isRunning, noclip, window);
	EnemyManager enemyManager = EnemyManager(&player, &map, &collision, isAIEnabled);
	Physics physics = Physics(&player, &enemyManager, &collision);
	Textures textures = Textures();
	std::map<int, SDL_Texture*> textureList = textures.PreLoadTextures(renderer);
	Graphics graphics = Graphics(&player, &camera, &collision, &enemyManager, &map, renderer, textureList);

	// Main game loop
	while (isRunning) {
		// Calculations to limit FPS
		fpsManager.Tick();

		if (enemyManager.enemies.size() == 0) {
			map.CreateRandomMap();
			map.SpawnEnemies();
			player.OnDeath();
			enemyManager = EnemyManager(&player, &map, &collision, isAIEnabled);
			collision = Collision(&player, &map, renderBounds);
		}

		if (delta >= 1) { // Do calculations for the frames that we use
			input.Tick();
			physics.Tick();
			player.Tick();
			enemyManager.TickEnemies();
			delta = 0;
		}

		graphics.Render(enemyManager.enemies.size());
	}

	// Delete things when we don't need them anymore
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