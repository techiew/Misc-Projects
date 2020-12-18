#define _USE_MATH_DEFINES //Needed for the use of M_PI (which is PI)

#include <iostream>
#include <math.h>
#include "Branch.h"

int Branch::numOfBranches = 0;

// Constructor used for trunks, the trunk is just a vertical line, so it doesn't need as many arguments
Branch::Branch(Vector2 start, int length) { 

	Branch::numOfBranches++;
	this->hasChildren = false;
	this->BranchLength = getRand(length/3, length); // Get random length
	this->rightAngle = 0;
	this->leftAngle = 0;

	this->generation = 0; // The tree starts with generation 0

	this->start.set(start.x, start.y); // The trunk starts at the origin
	this->end.set(start.x, start.y - this->BranchLength); // And the end of the trunk is equal to the start + its length

	// The trunk is completely black
	this->treec1 = 0; 
	this->treec2 = 0;
	this->treec3 = 0;

	// The leaves of this tree are given a random color
	// 50 to 255 gives us nice, strong colors
	this->leavesc1 = getRand(50, 255);
	this->leavesc2 = getRand(50, 255); 
	this->leavesc3 = getRand(50, 255);

}

// Constructor for branches, this constructor needs alot of arguments so that it can pass them on to its own children
Branch::Branch(Vector2 start, Vector2 end, int length, float angle, int generation, int treec1, int treec2, int treec3, int leavesc1, int leavesc2, int leavesc3, int generations) { 
	
	Branch::numOfBranches++;
	this->hasChildren = false;
	this->BranchLength = length;
	this->rightAngle = angle; // Set this branches' angles to what it was given by its parent
	this->leftAngle = angle;

	this->generation = generation+1; // This Branch is one higher generation than its parent

	this->start.set(start.x, start.y); // Set its start and end to whatever the parent supplied
	this->end.set(end.x, end.y);

	// RGB colors for the leaves of this tree
	this->leavesc1 = leavesc1;
	this->leavesc2 = leavesc2;
	this->leavesc3 = leavesc3;

	// Color of branches increases slightly towards leaf color for every generation
	this->treec1 = treec1+(leavesc1/14)*generation;
	this->treec2 = treec2+(leavesc2/14)*generation; 
	this->treec3 = treec3+(leavesc3/14)*generation;

	if (generation > generations) { 
		// If the Branch is a leaf, set the tree color to be a direct copy of leaf color
		// This code doesn't actually do anything as far as I remember
		this->treec1 = leavesc1;
		this->treec2 = leavesc2;
		this->treec3 = leavesc3;
	}

}

Branch Branch::createChild(int maxLength, bool dir, int generation, int generations) {
	int childGen = generation + 1; // Get generation of the children
	int length; // The random length of this Branch/leaf
	Vector2 drawToPos; // Vector that tells us where the Branch will draw to (From end vector to drawToPos vector)

	if (childGen > generations) { // Is this a normal Branch or leaf?
		length = getRand(0, 50);
		drawToPos.set(0, length);
	} else {
		length = getRand(10, maxLength);
		drawToPos.set(0, length);
	}

	if (childGen > generations) { // Is this a normal Branch or leaf?

		if (dir) { // Change rotation angle according to Branch direction, dir is a boolean.
			// True = left, false = right
			// 42.0f*(M_PI / 180.0f) = 42 degrees converted to radians
			this->leftAngle -= getRandFloat(0.0f, 42.0f*(M_PI / 180.0f)); // The angle variation is doubled (21 to 42), makes it seem more chaotic and leaf-like
			drawToPos.rotate(this->leftAngle);
		} else {
			this->rightAngle += getRandFloat(0.0f, 42.0f*(M_PI / 180.0f));
			drawToPos.rotate(this->rightAngle);
		}

	} else { // Do the same for normal branches

		if (dir) { 
			this->leftAngle -= getRandFloat(0.0f, 21.0f*(M_PI / 180.0f));
			drawToPos.rotate(this->leftAngle);
		} else {
			this->rightAngle += getRandFloat(0.0f, 21.0f*(M_PI / 180.0f));
			drawToPos.rotate(this->rightAngle);
		}

	}

	// Note: drawToPos.rotate() is a function in the Vector2 class and rotates a 2d vector by some amount.

	// Modify the pos a little bit...
	drawToPos.x += this->end.x;
	drawToPos.y = this->end.y - drawToPos.y;

	/*
	According to direction, pass on left or right angle
	All right branches will share the rightAngle variable but change it sligthly and pass it on
	the same goes for left branches
	*/

	if (dir) {
		Branch child(this->end, drawToPos, length, this->leftAngle, generation, this->treec1, this->treec2, this->treec3, this->leavesc1, this->leavesc2, this->leavesc3, generations);
		return child;
	} else {
		Branch child(this->end, drawToPos, length, this->rightAngle, generation, this->treec1, this->treec2, this->treec3, this->leavesc1, this->leavesc2, this->leavesc3, generations);
		return child;
	}

}

// Get a random float number in a specific range
float Branch::getRandFloat(float fMin, float fMax) {
	float f = fMin + (rand()) / ((float)(RAND_MAX / (fMax - fMin)));
	return f;
}

// Get a random integer in a specific range
int Branch::getRand(int rMin, int rMax) {
	int r = rand() % (rMax - rMin + 1) + rMin;
	return r;
}

Branch::~Branch() {

}