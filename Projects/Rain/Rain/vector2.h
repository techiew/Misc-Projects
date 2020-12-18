#pragma once
#ifndef VECTOR2_H
#define VECTOR2_H
#include <math.h>

class vector2 { 
public:
	int x, y;
	vector2();
	vector2(int a, int b);
	void set(int a, int b);
	void rotate(float angle_r);
};

#endif