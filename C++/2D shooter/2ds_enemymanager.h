#pragma once

#include "2ds_enemy.h"
#include "2ds_map.h"
#include "2ds_collision.h"

extern Map* gMap;
extern Collision* gCollision;

class EnemyManager {
public:
	int shootDistance;
	int aggroRange;
	int meleeHitDistance;
	int rangedClosestDistance;
	int enemyShootCooldown;
	bool isAIEnabled;
	std::vector<Enemy> enemies;
	std::vector<Projectile> enemyBullets;

	void TickEnemies();
	EnemyManager(bool isAIEnabled);
	~EnemyManager();
};