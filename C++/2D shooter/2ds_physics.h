#pragma once

#include "2ds_player.h"
#include "2ds_projectile.h"
#include "2ds_collision.h"
#include "2ds_enemy.h"
#include "2ds_enemymanager.h"

extern EnemyManager* gEnemyManager;

class Physics {
public:
	void Tick();
	void PlayerBulletPhysics();
	void EnemyBulletPhysics();
	Physics();
	~Physics();
};