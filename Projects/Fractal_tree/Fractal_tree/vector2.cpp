#include "Vector2.h"

Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}

Vector2::Vector2(int a, int b){
	this->x = a;
	this->y = b;
}

void Vector2::set(int a, int b) {
	this->x = a;
	this->y = b;
}

void Vector2::rotate(float angle_r) { 
	float _x = static_cast<float>(x),
		_y = static_cast<float>(y),
		s = sinf(angle_r),
		c = cosf(angle_r),
		a = _x * c - _y * s,
		b = _x * s + _y * c;
	this->x = static_cast<int>(a);
	this->y = static_cast<int>(b);
}