#include "room.h"

Room::Room(Vector2 origin, Vector2 bounds) {
	this->origin = origin;
	this->bounds = bounds;
	this->exits.clear();
	this->north = false;
	this->south = false;
	this->east = false;
	this->west = false;
}

void Room::AddExit(Vector2 exit) {
	exits.push_back(exit);
}

Vector2 Room::GetBranchPoint() {
	int i = GetRandInt(0, exits.size());
	Vector2 exitPos = exits[i];
	exits.erase(exits.begin() + i);
	return exitPos;
}

Room::~Room() {

}