#pragma once
#include <iostream>
#include "vector2.h"
#include "projectile.h"
#include "tile.h"
#include "enemy.h"
#include "player.h"

class Collision {
public:
	Player* player;
	Map* map;
	bool renderBounds;
	std::vector<Tile> walls;
	std::vector<std::pair<SDL_Rect, bool>> boxes;

	bool CheckEnemyHit(Enemy enemy);
	bool CheckPlayerHit(Projectile enemyBullet);
	void AddBoundingBox(Vector2 pos, int height, int width, bool isColliding);
	void UpdateWalls();
	bool RayCastCollision(Vector2 point);
	bool CollisionWithWalls(Vector2 movingToPos);
	Collision(Player* player, Map* map, bool renderBounds);
};