#pragma once
#ifndef BRANCH_H
#define BRANCH_H
#include <iostream>
#include "vector2.h" //Class used to store 2D position values

class branch {
public:
	static int numOfBranches; //Total number of branch objects
	vector2 start;
	vector2 end;
	int branchLength;
	int generation;
	float rightAngle; //Keeps track of the accumulating rotation angle of right branches
	float leftAngle; //Keeps track of the accumulating rotation angle of left branches
	bool hasChildren; //Does the branch have child branches?

	int treec1; //Tree color
	int treec2;
	int treec3;

	int leavesc1; //Color of leaves
	int leavesc2;
	int leavesc3;

	int getRand(int rMin, int rMax);
	float getRandFloat(float fMin, float fMax);
	branch createChild(int length, bool dir, int generation, int generations);
	branch(vector2 start, int length); //Constructor for trunk
	branch(vector2 start, vector2 end, int length, float angle, int generation, int brac1, int brac2, int brac3, int leavesc1, int leavesc2, int leavesc3, int leafLoops); //Constructor for any other branch
	~branch();
};

#endif