#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include "vector2.h"
#include "projectile.h"
#include "map.h"
#include "camera2d.h"

class Camera2D;

class Player {
public:
	Map* map;
	Camera2D* camera;

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
	Player(Map* map, Camera2D* camera, int playerSize, int maxHP, double movementPerTick, bool godmode);
	~Player();
};