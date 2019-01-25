#include <iostream>
#include "2ds_shadows.h"

Shadows::Shadows() {
	this->wallPoints.clear();
	this->shadowPoints.clear();
	this->maxRange = 1000;
	this->maxSteps = 50;
	CreateWallPoints(gMap->currentLevelWalls);
}

void Shadows::RayCast(Vector2 origin, Vector2 target, double angle, int rangeCounter) {

	/*float dirX = (wallPoint.pos.x - origin.x) / d;
	float dirY = (wallPoint.pos.y - origin.y) / d;*/

	Vector2 direction((float)cos(angle), (float)sin(angle));

	rangeCounter = (rangeCounter > 0) ? rangeCounter : 0;

	float checkX = origin.x;
	float checkY = origin.y;

	double stepRateX = 10;
	double stepRateY = 10;

	bool isFinished = false;

	while (!isFinished) {
		if (rangeCounter > maxSteps) {
			shadowPoints.push_back(std::make_pair(Vector2(checkX, checkY), std::atan2(checkX - origin.x, checkY - origin.y)));
			isFinished = true;
			return;
		} else if (checkX == target.x && checkY == target.y){
			shadowPoints.push_back(std::make_pair(Vector2(checkX, checkY), std::atan2(checkX - origin.x, checkY - origin.y)));
			isFinished = true;
			return;
			RayCast(Vector2(checkX, checkY), Vector2(0, 0), angle + 0.1, rangeCounter);
		} else //if ((int)checkX % 64 == 0 || (int)checkY % 64 == 0) {
			if (gCollision->RayCastCollision(Vector2(checkX, checkY))) {
				shadowPoints.push_back(std::make_pair(Vector2(checkX, checkY), std::atan2(checkX - origin.x, checkY - origin.y)));
				isFinished = true;
				return;
			}
	//}
		rangeCounter++;
		checkX += direction.x * stepRateX;
		checkY += direction.y * stepRateY;
	}
}


/*int x0 = gPlayer->pos.x;
int y0 = gPlayer->pos.y;
int x1 = wallPoint.x;
int y1 = wallPoint.y;

int slope = 0;
int pitch = 0;

if (x0 == x1 && y0 == y1) {
shadowPoints.push_back(Vector2(x0, y0));
return;
}

int dx = x1 - x0;
int sx = (dx < 0) ? -1 : 1;
int dy = y1 - y0;
int sy = (dy < 0) ? -1 : 1;

if (abs(dy) < abs(dx)) {
slope = dy / dx;
pitch = y0 - slope * x0;
while (x0 != x1) {
x0 += sx;
}
} else {
slope = dx / dy;
pitch = x0 - slope * y0;
while (y0 != y1) {
y0 += sy;
}
}

shadowPoints.push_back(Vector2(x1, y1));*/

void Shadows::CreateWallPoints(std::vector<Tile> walls) {
	wallPoints.clear();
	for (int i = 0; i < walls.size(); i++) {
		int width = walls[i].tileWidth;
		int height = walls[i].tileHeight;
		int posX = walls[i].position.x;
		int posY = walls[i].position.y;

		wallPoints.push_back(WallPoint(walls[i].position)); //X1 and Y1 (upper left)

		int wallX2 = posX + width;
		int wallY2 = posY;
		wallPoints.push_back(WallPoint(Vector2(wallX2, wallY2))); //upper right
		
		int wallX3 = posX;
		int wallY3 = posY + height;
		wallPoints.push_back(WallPoint(Vector2(wallX3, wallY3))); //lower left

		int wallX4 = posX + width;
		int wallY4 = posY + height;
		wallPoints.push_back(WallPoint(Vector2(wallX4, wallY4))); //Lower right

	}
}

double Shadows::GetAngleRadians(Vector2 direction) {
	//double angle = std::atan2(direction.y - gPlayer->pos.y, direction.x - gPlayer->pos.x);
	/*double degrees = (180 * angle / M_PI);
	double radians = (degrees * M_PI / 180);*/
	return std::atan2(direction.y - gPlayer->pos.y, direction.x - gPlayer->pos.x);
}

//bool SortByDistance (const WallPoint* wp0, const WallPoint* wp1) {
//	return wp0->d < wp1->d;
//}

struct SortByAngle {
	bool operator()(const std::pair<Vector2, double> &left, const std::pair<Vector2, double> &right) {
		return left.second > right.second;
	}
};

std::vector<std::pair<Vector2, double>> Shadows::GetShadowPoints() {
	/*for (int i = 0; i < wallPoints.size(); i++) {
		wallPoints[i].UpdatePoint(gPlayer->pos.x, gPlayer->pos.y);
	}*/

	for (int i = 0; i < wallPoints.size(); i++) {
		double vecX = wallPoints[i].pos.x;
		double vecY = wallPoints[i].pos.y;
		float d = sqrt(((vecX - gPlayer->pos.x) * (vecX - gPlayer->pos.x)) + ((vecY - gPlayer->pos.y) * (vecY - gPlayer->pos.y)));
		if (d > maxRange) { continue; }

		RayCast(gPlayer->pos, wallPoints[i].pos, GetAngleRadians(Vector2(vecX, vecY)), 0);
	}

	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x - maxRange, gPlayer->pos.y - maxRange), GetAngleRadians(Vector2(gPlayer->pos.x - maxRange, gPlayer->pos.y - maxRange)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x + maxRange, gPlayer->pos.y - maxRange), GetAngleRadians(Vector2(gPlayer->pos.x + maxRange, gPlayer->pos.y - maxRange)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x + maxRange, gPlayer->pos.y + maxRange), GetAngleRadians(Vector2(gPlayer->pos.x + maxRange, gPlayer->pos.y + maxRange)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x - maxRange, gPlayer->pos.y + maxRange), GetAngleRadians(Vector2(gPlayer->pos.x - maxRange, gPlayer->pos.y + maxRange)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x - maxRange, gPlayer->pos.y), GetAngleRadians(Vector2(gPlayer->pos.x - maxRange, gPlayer->pos.y-1)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x + maxRange, gPlayer->pos.y), GetAngleRadians(Vector2(gPlayer->pos.x + maxRange, gPlayer->pos.y+1)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x, gPlayer->pos.y + maxRange), GetAngleRadians(Vector2(gPlayer->pos.x+1, gPlayer->pos.y + maxRange)), 0);
	RayCast(gPlayer->pos, Vector2(gPlayer->pos.x, gPlayer->pos.y - maxRange), GetAngleRadians(Vector2(gPlayer->pos.x-1, gPlayer->pos.y - maxRange)), 0);

	std::sort(shadowPoints.begin(), shadowPoints.end(), SortByAngle());
	return this->shadowPoints;
}

Shadows::~Shadows() {

}