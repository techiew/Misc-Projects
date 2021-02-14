#include <vector>
#include <iostream>
#include "physics.h"

Physics::Physics(Player* player, EnemyManager* enemyManager, Collision* collision) {
	this->player = player;
	this->enemyManager = enemyManager;
	this->collision = collision;
}

void Physics::Tick() {
	PlayerBulletPhysics();
	EnemyBulletPhysics();
}

void Physics::PlayerBulletPhysics() {
	std::vector<Projectile> projectiles = player->projectiles;
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].Update();
		if (collision->CollisionWithWalls(Vector2(projectiles[i].x, projectiles[i].y))) {
			projectiles.erase(projectiles.begin() + i);
		}
		else if (projectiles[i].frames >= 600) {
			projectiles.erase(projectiles.begin() + i);
		}
	}
	player->projectiles = projectiles;
}

void Physics::EnemyBulletPhysics() {
	int bulletSize = 1;
	std::vector<Projectile> projectiles = enemyManager->enemyBullets;
	for (int i = 0; i < projectiles.size(); i++) {
		int bulletX = projectiles[i].x;
		int bulletY = projectiles[i].y;
		int playerX = player->pos.x;
		int playerY = player->pos.y;
		projectiles[i].Update();
		if (collision->CheckPlayerHit(projectiles[i])) {
			player->OnHit();
			projectiles.erase(projectiles.begin() + i);
		}else if (collision->CollisionWithWalls(Vector2(projectiles[i].x, projectiles[i].y))) {
			projectiles.erase(projectiles.begin() + i);
		} else if (projectiles[i].frames >= 600) {
			projectiles.erase(projectiles.begin() + i);
		}
	}
	enemyManager->enemyBullets = projectiles;
}