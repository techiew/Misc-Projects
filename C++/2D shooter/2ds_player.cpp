#include "2ds_player.h"

Player::Player(int playerSize, int maxHP, double movementPerTick, bool godmode) {
	this->playerSize = playerSize;
	this->pos = gMap->spawnpoint;
	this->currentHP = maxHP;
	this->maxHP = maxHP;
	this->aimAngle = Vector2(0, 0);
	this->aimPos = Vector2(0, 0);
	this->shootCooldown = false;
	this->godmode = godmode;
	this->projectiles.clear();
	this->mag = 15;
	this->bulletSize = 64;
	this->bulletSpeed = 30;
	this->movementPerTick = movementPerTick;
	this->invincible = false;
	this->thisHit = std::chrono::system_clock::now();
}

Projectile Player::Shoot() {
	Projectile newProjectile(Vector2(aimAngle.x, aimAngle.y), Vector2((aimAngle.x-pos.x)/mag, (aimAngle.y-pos.y)/mag), bulletSize, bulletSpeed);
	projectiles.push_back(newProjectile);
	shootCooldown = true;
	return newProjectile;
}

void Player::OnHit() {
	if (godmode) {
		return;
	}

	if (!invincible) {
		lastHit = thisHit;
		currentHP -= 1;
		invincible = true;
	}

	if (currentHP <= 0) {
		OnDeath();
	}
}

void Player::OnDeath() {
	SetPos(Vector2(spawnPoint.x + 100, spawnPoint.y + 100));
	currentHP = maxHP;
}

void Player::Tick() {
	thisHit = std::chrono::system_clock::now();
	elapsedTime = thisHit - lastHit;
	if (elapsedTime.count() >= 2) {
		invincible = false;
	}

}

void Player::SetAimPos(Vector2 mousePos) {
	aimPos.Set(mousePos.x, mousePos.y);
	Vector2 mouseAngle(mousePos.x - (pos.x - gCamera->offset.x), mousePos.y - (pos.y - gCamera->offset.y));
	double angle = std::atan2(mouseAngle.y, mouseAngle.x);
	aimAngle = Vector2(pos.x + std::cos(angle)*mag, pos.y + std::sin(angle)*mag);
}

void Player::SetPos(Vector2 pos) {
	this->pos = pos;
}

Player::~Player() {

}