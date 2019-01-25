#pragma once

#include <SDL.h>
#include <vector>
#include <SDL2_gfxPrimitives.h>
#include <map>
#include <math.h>
#include <iostream>
#include "2ds_vector2.h"
#include "2ds_tile.h"
#include "2ds_projectile.h"
#include "2ds_enemy.h"
#include "2ds_map.h"
#include "2ds_camera2d.h"
#include "2ds_shadows.h"
#include "2ds_enemymanager.h"
#include <SDL_ttf.h>

extern EnemyManager* gEnemyManager;
extern Shadows* gShadows;

class Graphics {
public:
	SDL_Renderer* renderer;
	SDL_Rect srcRect; //SDL_Rect used to store textures
	SDL_Rect bgTextRect;
	std::map<int, SDL_Texture*> textures;
	int boxCounter;
	int blinkCount;
	TTF_Font* font;
	int numEnemies;

	/*void AddBoundingBox(Vector2 fromPos, Vector2 toPos, int size, bool isColliding);*/
	void AddShadowPoint(Vector2 shadowPoint);
	SDL_Rect CreateRect(int x, int y, int w, int h);
	void Render(int numEnemies);
	void RenderText(std::string text, int x, int y, int w, int h, SDL_Color color);
	Graphics(SDL_Renderer* renderer, std::map<int, SDL_Texture*> textures);
	~Graphics();
};