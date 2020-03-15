#include <iostream>
#include "rand.h"

int GetRandInt(int min, int max) {
	int r = rand() % (max - min + 1) + min;
	return r;
}

float GetRandFloat(float min, float max) {
	float r = (float)rand() / (RAND_MAX + 1) * (max - min) + min;
	return r;
}

double GetRandDouble(double min, double max) {
	//double r = rand() % (max - min + 1) + min;
	return 0.0;
}

int ChooseInt(std::vector<int> options) {
	int choice = GetRandInt(0, options.size() - 1);
	return options[choice];
}