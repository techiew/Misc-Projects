#include "2ds_enemy.h"

Enemy::Enemy(Vector2 spawnPoint, int type) {
	this->pos = spawnPoint;
	this->mag = 15;
	this->hitEffect = false;
	this->hp = 2;
	this->type = type;
	this->cooldownCount = 0;
	this->SetAimPos(Vector2(spawnPoint.x + GetRandInt(-100, 100), spawnPoint.y + GetRandInt(-100, 100)));
	this->shootCooldown = false;
	if (true) {
		this->enemySize = 64;
		this->enemySpeed = 10;
		this->bulletSize = 64;
		this->bulletSpeed = 15;
	}
}

Projectile Enemy::Shoot() {
	Projectile newProjectile(Vector2(aimAngle.x, aimAngle.y), Vector2((aimAngle.x - pos.x) / mag, (aimAngle.y - pos.y) / mag), bulletSize, bulletSpeed);
	shootCooldown = true;
	return newProjectile;
}

void Enemy::AddToPos(float addX, float addY) {
	pos.Set(pos.x + addX, pos.y + addY);
}

void Enemy::OnHit(Vector2 hitFromPos) {
	hitEffect = true;
	hp -= 1;

	if (type == 0) {
		float d = sqrt(((pos.x - hitFromPos.x) * (pos.x - hitFromPos.x)) + ((pos.y - hitFromPos.y) * (pos.y - hitFromPos.y)));
		float moveX = hitFromPos.x - pos.x;
		float moveY = hitFromPos.y - pos.y;
		knockback.Set((moveX / d)*75, (moveY / d)*75);
		AddToPos(-knockback.x, -knockback.y);
	}
}

void Enemy::SetAimPos(Vector2 aimPos) {
	int aimPosX = aimPos.x - pos.x;
	int aimPosY = aimPos.y - pos.y;
	float angle = std::atan2(aimPosY, aimPosX);
	aimAngle = Vector2(pos.x + std::cos(angle)*mag, pos.y + std::sin(angle)*mag);
}

Enemy::~Enemy() {

}