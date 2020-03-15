#pragma once

#include <math.h>

class Vector2 { 
public:
	double x, y;

	Vector2();
	Vector2(double x, double y);
	void Set(double x, double y);
	void SetX(double x);
	void SetY(double y);
	void Rotate(double angleInRadians);
};