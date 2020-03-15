#pragma once

#include <iostream>

class Particle {
public:
	std::pair <double, double> vel;
	std::pair <double, double> acc;
	std::pair <double, double> pos;
	std::pair <double, double> target;
	std::pair <double, double> dir;
	std::pair <double, double> result;
	double dsquared;
	double force;
	double G;

	void Update();
	void Attract(std::pair <double, double>);
	Particle(int posX, int posY);
	~Particle();
};