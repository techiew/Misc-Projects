#pragma once
#include "enemy.h"
#include "map.h"
#include "collision.h"

class EnemyManager {
public:
	Player* player;
	Map* map;
	Collision* collision;
	int shootDistance;
	int aggroRange;
	int meleeHitDistance;
	int rangedClosestDistance;
	int enemyShootCooldown;
	bool isAIEnabled;
	std::vector<Enemy> enemies;
	std::vector<Projectile> enemyBullets;

	void TickEnemies();
	EnemyManager(Player* player, Map* map, Collision* collision, bool isAIEnabled);
	~EnemyManager();
};