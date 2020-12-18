#pragma once
#include "vector2.h"
#include "player.h"
#include "tile.h"

class Player;

class Camera2D {
public:
	Player* player;
	Vector2 offset;
	int windowX, windowY;

	void SetPlayer(Player* player);
	void UpdateCamera(int mouseX, int mouseY);
	void UpdateWindowSize(int newX, int newY);
	bool IsTileVisible(Tile tile);
	Camera2D(int windowX, int windowY);
};
