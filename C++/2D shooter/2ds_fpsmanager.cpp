#include <iostream>
#include <SDL.h>
#include "2ds_fpsmanager.h"

FPSManager::FPSManager(int FPS, bool counter, double* delta) {
	this->FPS = FPS;
	this->counter = counter;
	this->currentTime = 0;
	this->lastTime = SDL_GetTicks();
	this->timePerTick = 1000 / FPS;
	this->timer = 0;
	this->ticks = 0;
	this->delta = delta;
}

void FPSManager::SetFPS(int FPS) {
	this->FPS = FPS;
	timePerTick = 1000 / FPS;
}

void FPSManager::Tick() {
	currentTime = SDL_GetTicks();
	*delta += (currentTime - lastTime) / timePerTick;

	if (counter) { //Print FPS to console every second if enabled
		timer += currentTime - lastTime;

		if (timer >= 1000) { 
			std::cout << "Frames per second: " << ticks << std::endl;
			ticks = 0;
			timer = 0;
		}

		if (*delta >= 1) {
			ticks++;
		}

	}

	lastTime = currentTime;
}

FPSManager::~FPSManager() {

}