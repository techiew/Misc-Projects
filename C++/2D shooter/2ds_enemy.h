#pragma once

#include <cmath>
#include <iostream>
#include "2ds_vector2.h"
#include "2ds_rand.h"
#include "2ds_projectile.h"

class Enemy {
public:
	int hp;
	int mag; //Magnitude of the aimAngle line
	int type;
	int enemySize;
	int enemySpeed;
	int bulletSize;
	int bulletSpeed;
	int cooldownCount;
	bool hitEffect;
	bool shootCooldown;
	Vector2 knockback;
	Vector2 pos;
	Vector2 aimAngle;
	Vector2 aimPos;

	Projectile Shoot();
	void AddToPos(float addX, float addY);
	void OnHit(Vector2 hitFromPos);
	void SetAimPos(Vector2 pos);
	Enemy(Vector2 spawnPoint, int type);
	~Enemy();
};