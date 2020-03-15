#pragma once

#include <iostream>
#include "Vector2.h"

class Branch {
public:
	static int numOfBranches; // Total number of Branch objects
	Vector2 start;
	Vector2 end;
	int BranchLength;
	int generation;
	float rightAngle; // Keeps track of the accumulating rotation angle of right branches
	float leftAngle; // Keeps track of the accumulating rotation angle of left branches
	bool hasChildren; // Does the Branch have child branches?

	int treec1; // Tree color
	int treec2;
	int treec3;

	int leavesc1; // Color of leaves
	int leavesc2;
	int leavesc3;

	int getRand(int rMin, int rMax);
	float getRandFloat(float fMin, float fMax);
	Branch createChild(int length, bool dir, int generation, int generations);
	Branch(Vector2 start, int length);
	Branch(Vector2 start, Vector2 end, int length, float angle, int generation, int brac1, int brac2, int brac3, int leavesc1, int leavesc2, int leavesc3, int leafLoops); //Constructor for any other Branch
	~Branch();
};