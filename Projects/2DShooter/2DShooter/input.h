#pragma once
#include <sdl.h>
#include <iostream>
#include "vector2.h"
#include "camera2d.h"
#include "collision.h"

class Input {
public:
	Player* player;
	Camera2D* camera;
	Collision* collision;
	bool* isRunning;
	bool right, left, up, down, sprint;
	bool holdShoot;
	bool noclip;
	int cooldownCount;
	int moveX, moveY;
	SDL_Event event;
	SDL_Window* window;

	void Tick();
	Input(Player* player, Camera2D* camera, Collision* collision, bool* isRunning, bool noclip, SDL_Window* window);
	~Input();
};
