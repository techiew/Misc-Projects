#include "2ds_wallpoint.h"

WallPoint::WallPoint(Vector2 pos) {
	this->pos = pos;
	this->d = 0;
}

void WallPoint::UpdatePoint(float playerX, float playerY) {
	this->d = sqrt(((pos.x - playerX) * (pos.x - playerX)) + ((pos.y - playerY) * (pos.y - playerY)));
}

WallPoint::~WallPoint() {

}