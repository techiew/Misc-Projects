#pragma once

#include <math.h>

class Vector2 { 
public:
	int x, y;
	Vector2();
	Vector2(int a, int b);
	void set(int a, int b);
	void rotate(float angle_r);
};