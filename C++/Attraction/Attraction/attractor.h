#pragma once
#ifndef attractor_h
#define attractor_h

#include <iostream>

class attractor {

private:
	static int numOfAttractors;
	static double getRandDouble(double, double);

public:
	std::pair <double, double> pos;
	double speed;
	double dir;
	int resX;
	int resY;
	void update();
	attractor(double, double);
	attractor(int resX, int resY);
	~attractor();
};

#endif