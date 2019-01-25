#include "2ds_enemymanager.h"

EnemyManager::EnemyManager(bool isAIEnabled) {
	this->enemies = gMap->currentLevelEnemies;
	this->enemyBullets.clear();
	this->isAIEnabled = isAIEnabled;

	//Enemy options
	this->shootDistance = 750;
	this->aggroRange = 750;
	this->meleeHitDistance = 40;
	this->rangedClosestDistance = 250;
	this->enemyShootCooldown = 30;
}

void EnemyManager::TickEnemies() {

	for (int i = 0; i < enemies.size(); i++) {

		float enemyX = enemies[i].pos.x;
		float enemyY = enemies[i].pos.y;
		float moveX = 0;
		float moveY = 0;
		float dirX = 0;
		float dirY = 0;
		int addX = 0;
		int addY = 0;
		int randX = 0;
		int randY = 0;
		int enemySpeed = enemies[i].enemySpeed;

		//Check for collision with the player's bullets
		if (gCollision->CheckEnemyHit(enemies[i])) {
			//Trigger OnHit event
			enemies[i].OnHit(gPlayer->pos);
		}

		//Kill enemy if HP is 0
		if (enemies[i].hp <= 0) {
			this->enemies.erase(enemies.begin() + i);
			continue;
		}

		//Update position after knockback in OnHit() function
		enemyX = enemies[i].pos.x;
		enemyY = enemies[i].pos.y;

		//Unstuck enemy if enemy is inside wall after knockback
		if (gCollision->CollisionWithWalls(Vector2(enemyX, enemyY))) {
			enemies[i].AddToPos(enemies[i].knockback.x, enemies[i].knockback.y);
		}

		//Skip if AI is not enabled
		if (!isAIEnabled) {
			continue;
		}

		//Get distance between player and enemy
		float d = sqrt(((enemyX - gPlayer->pos.x) * (enemyX - gPlayer->pos.x)) + ((enemyY - gPlayer->pos.y) * (enemyY - gPlayer->pos.y)));

		int enemyType = enemies[i].type;

		//Idle behavior (independent of distance to player)
		switch (enemyType) {
		case 0: //Melee enemies
			//Add random jittering effect
			randX = GetRandInt(-enemySpeed, enemySpeed);
			randY = GetRandInt(-enemySpeed, enemySpeed);

			break;

		case 1: //Shooting enemies
			//Add random jittering effect
			randX = GetRandInt(-enemySpeed, enemySpeed);
			randY = GetRandInt(-enemySpeed, enemySpeed);
			break;
		}

		//Active behavior (when player is close enough)
		switch (enemyType) {
		case 0: //Melee enemies

			if (d > aggroRange) {
				break;
			}

			//Where enemy needs to move
			moveX = gPlayer->pos.x - enemyX;
			moveY = gPlayer->pos.y - enemyY;

			//Normalize the vector and multiply by magnitude (enemySpeed)
			dirX = (moveX / d)*enemySpeed;
			dirY = (moveY / d)*enemySpeed;

			break;

		case 1: //Shooting enemies

			if (d > shootDistance) {
				break;
			}

			//Where enemy needs to move
			moveX = gPlayer->pos.x - enemyX;
			moveY = gPlayer->pos.y - enemyY;

			//Normalize the vector and multiply by magnitude (enemySpeed)
			dirX = (moveX / d)*enemySpeed;
			dirY = (moveY / d)*enemySpeed;

			//Make enemy aim at player
			enemies[i].SetAimPos(gPlayer->pos);

			//Shoot if cooldown is not active
			if (enemies[i].shootCooldown == false && d < shootDistance) {
				enemyBullets.push_back(enemies[i].Shoot());
			} else {
				//Check shooting cooldown
				if (enemies[i].cooldownCount >= enemyShootCooldown) {
					enemies[i].shootCooldown = false;
					enemies[i].cooldownCount = 0;
				} else {
					enemies[i].cooldownCount++;
				}
			}

			break;
		}

		//Apply enemy movement
		switch (enemyType) {
		case 0: //Melee enemies

			if (d > meleeHitDistance) {
				addX = dirX + randX;
				addY = dirY + randY;
			} else {
				addX = randX;
				addY = randY;
				gPlayer->OnHit();
			}

			if (!gCollision->CollisionWithWalls(Vector2(enemyX + addX, enemyY + addY))) {
				enemies[i].AddToPos(addX, addY);
			} else {
				enemies[i].AddToPos(-addX, -addY);
			}

			break;

		case 1: //Shooting enemies

			if (d > rangedClosestDistance) {
				addX = dirX;
				addY = dirY;
			} else {
				addX = -dirX + randX;
				addY = -dirY + randY;
			}

			if (!gCollision->CollisionWithWalls(Vector2(enemyX + addX, enemyY + addY))) {
				enemies[i].AddToPos(addX, addY);
			}

			break;
		}
	}
}

EnemyManager::~EnemyManager() {

}