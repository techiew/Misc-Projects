#pragma once

#include "2ds_vector2.h"
#include <vector>
#include <math.h>

class WallPoint {
public:
	Vector2 pos;
	double angle;
	float d;
	bool operator <(WallPoint wp) { return d < wp.d; }
	void UpdatePoint(float playerX, float playerY);
	WallPoint(Vector2 pos);
	~WallPoint();
};