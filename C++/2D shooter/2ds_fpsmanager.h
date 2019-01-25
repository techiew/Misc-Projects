#pragma once

class FPSManager {
public:
	int FPS;
	int ticks;
	double timer;
	double currentTime;
	double lastTime;
	double timePerTick;
	double* delta;
	bool counter;

	void SetFPS(int FPS);
	void Tick();
	FPSManager(int FPS, bool counter, double* delta);
	~FPSManager();
};