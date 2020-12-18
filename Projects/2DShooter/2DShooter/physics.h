#pragma once
#include "player.h"
#include "projectile.h"
#include "collision.h"
#include "enemy.h"
#include "enemymanager.h"

class Physics {
public:
	Player* player;
	EnemyManager* enemyManager;
	Collision* collision;
	void Tick();
	void PlayerBulletPhysics();
	void EnemyBulletPhysics();
	Physics(Player* player, EnemyManager* enemyManager, Collision* collision);
};