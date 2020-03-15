#pragma once

#include "vector2.h"

class Projectile {
public:
	int frames;
	int bulletSize;
	double x, y;
	double bulletSpeed;
	Vector2 shootDirection;

	void Update();
	Projectile(Vector2 pos, Vector2 direction, int bulletSize, double bulletSpeed);
	~Projectile();
};