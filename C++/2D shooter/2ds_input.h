#pragma once

#include <sdl.h>
#include <iostream>
#include "2ds_vector2.h"
#include "2ds_camera2d.h"
#include "2ds_collision.h"

extern Camera2D* gCamera;
extern Collision* gCollision;

class Input {
public:
	bool* isRunning;
	bool right, left, up, down, sprint;
	bool holdShoot;
	bool noclip;
	int cooldownCount;
	int moveX, moveY;
	SDL_Event event;
	SDL_Window* window;

	void Tick();
	Input(bool* isRunning, bool noclip, SDL_Window* window);
	~Input();
};
