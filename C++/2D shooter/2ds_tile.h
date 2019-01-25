#pragma once

#include "2ds_vector2.h"
#include <SDL.h>
#include <string>
#include <vector>

class Tile {
public:
	static enum tileType {
		GREYFLOOR,
		GREYWALL,
		GREYWALLCORNER,
		ENEMYSPAWN,
		ENEMYSHOOTER,
		ENEMYSHOOTER_DAMAGED,
		ENEMYMELEE,
		ENEMYMELEE_DAMAGED,
		PLAYERBULLET,
		ENEMYBULLET,
		PLAYER,
		BLUEWALL,
		BLUEWALLSIDE,
		BLUEWALLSINGLE,
		BLUEWALLCORNER,
		BLUEWALLEDGE_R,
		BLUEWALLEDGE_L,
		CROSSHAIR
	};

	static enum tileRotation {
		DOWN = 0,
		RIGHT = -90,
		LEFT = 90,
		UP = 180
	};

	std::vector<Tile::tileType> collidableTiles = {
		GREYWALL,
		BLUEWALL,
		BLUEWALLSIDE,
		BLUEWALLSINGLE,
		BLUEWALLEDGE_R,
		BLUEWALLEDGE_L,
		BLUEWALLCORNER
	};

	std::vector<Tile::tileType> invisibleTiles = {
		ENEMYSPAWN
	};

	int tileHeight;
	int tileWidth;
	int type;
	bool isCollidable;
	bool isVisible;
	tileRotation rotation;
	Vector2 position;
	SDL_Texture* texture = nullptr;
	SDL_Rect GetRect();
	Vector2 GetCenter();
	Tile(tileType typeOfTile, int tileHeight, int tileWidth, Vector2 position, Tile::tileRotation);
	~Tile();
};