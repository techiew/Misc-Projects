#pragma once
#include <SDL.h>
#include <vector>
#include <SDL2_gfxPrimitives.h>
#include <map>
#include <math.h>
#include <iostream>
#include "vector2.h"
#include "tile.h"
#include "projectile.h"
#include "enemy.h"
#include "map.h"
#include "camera2d.h"
#include "enemymanager.h"
#include <SDL_ttf.h>

class Graphics {
public:
	Player* player;
	Camera2D* camera;
	Collision* collision;
	EnemyManager* enemyManager;
	Map* map;

	SDL_Renderer* renderer;
	SDL_Rect srcRect;
	SDL_Rect bgTextRect;
	std::map<int, SDL_Texture*> textures;
	int boxCounter;
	int blinkCount;
	TTF_Font* font;
	int numEnemies;

	SDL_Rect CreateRect(int x, int y, int w, int h);
	void Render(int numEnemies);
	void RenderText(std::string text, int x, int y, int w, int h, SDL_Color color);
	Graphics(Player* player, Camera2D* camera, Collision* collision, EnemyManager* enemyManager, 
		Map* map, SDL_Renderer* renderer, std::map<int, SDL_Texture*> textures);
	~Graphics();
};