#include "vector2.h"

//2D vector class used for programs or games that need two-dimensional position, acceleration, or force vectors.
Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}

Vector2::Vector2(double x, double y){
	this->x = x;
	this->y = y;
}

void Vector2::Set(double x, double y) {
	this->x = x;
	this->y = y;
}

void Vector2::SetX(double x) {
	this->x = x;
}

void Vector2::SetY(double y) {
	this->y = y;
}

void Vector2::Rotate(double angleInRadians) { //Some complex looking math to rotate the 2d vector, this uses radians to rotate
	double _x = static_cast<double>(x),
		_y = static_cast<double>(y),
		s = sinf(angleInRadians),
		c = cosf(angleInRadians),
		a = _x * c - _y * s,
		b = _x * s + _y * c;
	this->x = static_cast<int>(a);
	this->y = static_cast<int>(b);
}