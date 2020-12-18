#include "attractor.h"
#include <iostream>

int attractor::numOfAttractors = 0;

attractor::attractor(int resX, int resY) { //Spawn attractor randomly witin the window
	attractor::numOfAttractors++; //Value that increments every time an attractor is made, useful if you want to count them
	this->resX = resX;
	this->resY = resY;
	pos = std::make_pair(getRandDouble(resX, 0), getRandDouble(resY, 0)); //Get random position
	speed = getRandDouble(0.2, 0.3); //Used if you want moving attractors
	dir = getRandDouble(0, 1); //Used if you want moving attractors
}

attractor::attractor(double posX, double posY) { //Spawn attractor at a specific point
	attractor::numOfAttractors++; //Value that increments every time an attractor is made, useful if you want to count them
	pos = std::make_pair(posX, posY); //Set the position to the given point
	speed = getRandDouble(0.2, 0.3); //Used if you want moving attractors
	dir = getRandDouble(0, 1); //Used if you want moving attractors
}

void attractor::update() { 	
	if (dir > 0.5) {
		this->pos.first += speed;
	}
	else {
		this->pos.first -= speed;
	}
	if (this->pos.first > 1200.0) {
		this->pos.first = 0.0;
	}else if(this->pos.first < 0.0) {
		this->pos.first = 1200.0;
	}
}

double attractor::getRandDouble(double dMin, double dMax) {
	double d = (double)rand() / (RAND_MAX + 1) * (dMax - dMin) + dMin;
	return d;
}

attractor::~attractor() {

}
