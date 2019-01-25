#pragma once
#include <vector>
#include "2ds_vector2.h"
#include "2ds_rand.h"

class Room {
public:
	Vector2 origin;
	Vector2 bounds;
	std::vector<Vector2> exits; //Vector of all exits that can be branched from
	bool north, south, east, west; //Have rooms been made in these directions relative to this room? (branching does not count)

	void AddExit(Vector2 exit); //Add exit to vector after map.cpp creates it
	Vector2 GetBranchPoint(); //Get pos of an exit for branching
	Room(Vector2 origin, Vector2 bounds);
	~Room();
};