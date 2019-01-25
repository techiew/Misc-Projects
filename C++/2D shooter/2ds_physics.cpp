#include <vector>
#include <iostream>
#include "2ds_physics.h"

Physics::Physics() {

}

void Physics::Tick() {
	PlayerBulletPhysics();
	EnemyBulletPhysics();
}

void Physics::PlayerBulletPhysics() {
	std::vector<Projectile> projectiles = gPlayer->projectiles;
	for (int i = 0; i < projectiles.size(); i++) {
		projectiles[i].Update();
		if (gCollision->CollisionWithWalls(Vector2(projectiles[i].x, projectiles[i].y))) {
			projectiles.erase(projectiles.begin() + i);
		}
		else if (projectiles[i].frames >= 600) {
			projectiles.erase(projectiles.begin() + i);
		}
	}
	gPlayer->projectiles = projectiles;
}

void Physics::EnemyBulletPhysics() {
	int bulletSize = 1;
	std::vector<Projectile> projectiles = gEnemyManager->enemyBullets;
	for (int i = 0; i < projectiles.size(); i++) {
		int bulletX = projectiles[i].x;
		int bulletY = projectiles[i].y;
		int playerX = gPlayer->pos.x;
		int playerY = gPlayer->pos.y;
		projectiles[i].Update();
		if (gCollision->CheckPlayerHit(projectiles[i])) {
			gPlayer->OnHit();
			projectiles.erase(projectiles.begin() + i);
		}else if (gCollision->CollisionWithWalls(Vector2(projectiles[i].x, projectiles[i].y))) {
			projectiles.erase(projectiles.begin() + i);
		} else if (projectiles[i].frames >= 600) {
			projectiles.erase(projectiles.begin() + i);
		}
	}
	gEnemyManager->enemyBullets = projectiles;
}

Physics::~Physics() {

}