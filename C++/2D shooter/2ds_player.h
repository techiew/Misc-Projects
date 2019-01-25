#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "2ds_vector2.h"
#include "2ds_projectile.h"
#include "2ds_map.h"
#include "2ds_camera2d.h"

class Camera2D;

extern Map* gMap;
extern Camera2D* gCamera;

class Player {
public:
	int mag; //Magnitude of the aimAngle line
	int currentHP;
	int maxHP;
	int playerSize;
	int bulletSize;
	int bulletSpeed;
	bool shootCooldown;
	bool godmode;
	bool invincible;
	double movementPerTick;
	std::vector<Projectile> projectiles;
	Vector2 pos;
	Vector2 aimAngle;
	Vector2 aimPos;
	Vector2 spawnPoint;
	std::chrono::time_point<std::chrono::system_clock> thisHit, lastHit;
	std::chrono::duration<double> elapsedTime;

	Projectile Shoot();
	void OnHit();
	void OnDeath();
	void Tick();
	void SetAimPos(Vector2 mousePos);
	void SetPos(Vector2 pos);
	Player(int playerSize, int maxHP, double movementPerTick, bool godmode);
	~Player();
};