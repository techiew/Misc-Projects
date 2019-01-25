#pragma once

#include <iostream>
#include "2ds_vector2.h"
#include "2ds_projectile.h"
#include "2ds_tile.h"
#include "2ds_enemy.h"
#include "2ds_player.h"

extern Player* gPlayer;

class Collision {
public:
	bool renderBounds;
	std::vector<Tile> walls;
	std::vector<std::pair<SDL_Rect, bool>> boxes;

	bool CheckEnemyHit(Enemy enemy);
	bool CheckPlayerHit(Projectile enemyBullet);
	void AddBoundingBox(Vector2 pos, int height, int width, bool isColliding);
	void UpdateWalls();
	bool RayCastCollision(Vector2 point);
	bool CollisionWithWalls(Vector2 movingToPos);
	Collision(bool renderBounds);
	~Collision();
};