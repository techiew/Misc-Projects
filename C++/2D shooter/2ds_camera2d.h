#pragma once

#include "2ds_vector2.h"
#include "2ds_player.h"
#include "2ds_tile.h"

class Player;

extern Player* gPlayer;

class Camera2D {
public:
	Vector2 offset;
	int windowX, windowY;

	void UpdateCamera(int mouseX, int mouseY);
	void UpdateWindowSize(int newX, int newY);
	bool IsTileVisible(Tile tile);
	Camera2D(int windowX, int windowY);
	~Camera2D();
};
