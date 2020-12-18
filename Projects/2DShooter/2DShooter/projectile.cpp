#include "projectile.h"
#include <iostream>

Projectile::Projectile(Vector2 pos, Vector2 direction, int bulletSize, double bulletSpeed) {
	this->x = pos.x;
	this->y = pos.y;
	this->shootDirection = direction;
	this->bulletSize = bulletSize;
	this->bulletSpeed = bulletSpeed;
	this->frames = 0;
}

void Projectile::Update() {
	this->x += shootDirection.x * bulletSpeed;
	this->y += shootDirection.y * bulletSpeed;
	this->frames++;
}

Projectile::~Projectile() {

}