#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
#include "2ds_vector2.h"
#include "2ds_wallpoint.h"
#include "2ds_tile.h"
#include "2ds_map.h"
#include "2ds_collision.h"

extern Map* gMap;
extern Collision* gCollision;

class Shadows {
public:
	int maxRange;
	int maxSteps;
	std::vector<WallPoint> wallPoints;
	std::vector<std::pair<Vector2, double>> shadowPoints;

	void RayCast(Vector2 origin, Vector2 target, double angle, int rangeCounter);
	void CreateWallPoints(std::vector<Tile> walls);
	double GetAngleRadians(Vector2 direction);
	std::vector<std::pair<Vector2, double>> GetShadowPoints();
	Shadows();
	~Shadows();
};