#include "2ds_graphics.h"

Graphics::Graphics(SDL_Renderer* renderer, std::map<int, SDL_Texture*> textures) {
	this->renderer = renderer;
	this->textures = textures;
	this->boxCounter = 0;
	this->srcRect.w = 64;
	this->srcRect.h = 64;
	this->srcRect.x = 0;
	this->srcRect.y = 0;
	this->bgTextRect.w = 2048;
	this->bgTextRect.h = 2048;
	this->bgTextRect.x = 0;
	this->bgTextRect.y = 0;
	this->blinkCount = 0;
	if (TTF_Init() < 0) {
		std::cout << "Error: " << TTF_GetError() << std::endl;
	}
	this->font = TTF_OpenFont((char*)"arial.ttf", 10);
	if (font == NULL) {
		std::cout << "Font loading error" << std::endl;
	}
	this->numEnemies = gMap->currentLevelEnemySpawns.size();
}

//void Graphics::AddBoundingBox(Vector2 fromPos, Vector2 toPos, int size, bool isColliding) {
//	SDL_Rect box;
//	box.x = fromPos.x;
//	box.y = fromPos.y;
//	box.w = toPos.x + size;
//	box.h = toPos.y + size;
//	this->boxes.push_back(box);
//	this->isBoxColliding.push_back(isColliding);
//}

//void Graphics::AddShadowPoint(Vector2 shadowPoint) {
//	this->shadowPoints.push_back(shadowPoint);
//}

SDL_Rect Graphics::CreateRect(int x, int y, int w, int h) {
	SDL_Rect newRect;
	newRect.x = x;
	newRect.y = y;
	newRect.w = w;
	newRect.h = h;
	return newRect;
}

void Graphics::Render(int numEnemies) {

	std::vector<Tile> tiles = gMap->currentLevelTiles;
	std::vector<Tile> walls = gMap->currentLevelWalls;
	std::vector<Projectile> enemyBullets = gEnemyManager->enemyBullets;
	std::vector<Enemy> enemies = gEnemyManager->enemies;
	std::vector<std::pair<Vector2, double>> shadowPoints; //= gShadows->GetShadowPoints();

	double offsetX = gCamera->offset.x;
	double offsetY = gCamera->offset.y;

	int playerX = gPlayer->pos.x - offsetX;
	int playerY = gPlayer->pos.y - offsetY;
	int playerSize = gPlayer->playerSize;
	int aimAngleX = gPlayer->aimAngle.x - offsetX;
	int aimAngleY = gPlayer->aimAngle.y - offsetY;
	int aimPosX = gPlayer->aimPos.x;
	int aimPosY = gPlayer->aimPos.y;

	//Draw floor
	for (int i = 0; i < tiles.size(); i++) {
	
		if (!tiles[i].isVisible) { continue; }
		//if (!this->camera->IsVisible(tiles[i], this->player)) { continue; }

		tiles[i].position = Vector2(tiles[i].position.x - offsetX, tiles[i].position.y - offsetY);
		SDL_Rect tileRect = tiles[i].GetRect();
		int tileType = tiles[i].type;
		double tileRotation = tiles[i].rotation;

		if (tileRotation == Tile::DOWN) {
			SDL_RenderCopy(renderer, textures[tileType], &srcRect, &tileRect);
		} else if (tileRotation == Tile::RIGHT) {
			SDL_RenderCopyEx(renderer, textures[tileType], &srcRect, &tileRect, (double)tileRotation, NULL, SDL_FLIP_NONE);
		} else if (tileRotation == Tile::LEFT) {
			SDL_RenderCopyEx(renderer, textures[tileType], &srcRect, &tileRect, (double)tileRotation, NULL, SDL_FLIP_NONE);
		} else { 
			SDL_RenderCopyEx(renderer, textures[tileType], &srcRect, &tileRect, (double)tileRotation, NULL, SDL_FLIP_NONE);
		}
	}

	//Draw walls (drawn after floor, so that the walls are always on top)
	for (int i = 0; i < walls.size(); i++) {

		//if (!walls[i].isVisible) { continue; }
		//if (!this->camera->IsVisible(walls[i], this->player)) { continue; }

		walls[i].position = Vector2(walls[i].position.x - offsetX, walls[i].position.y - offsetY);
		SDL_Rect wallRect = walls[i].GetRect();
		int wallType = walls[i].type;
		double wallRotation = walls[i].rotation;

		if (wallRotation == Tile::DOWN) {
			SDL_RenderCopy(renderer, textures[wallType], &srcRect, &wallRect);
		} else if (wallRotation == Tile::RIGHT) {
			SDL_RenderCopyEx(renderer, textures[wallType], &srcRect, &wallRect, (double)wallRotation, NULL, SDL_FLIP_NONE);
		} else if (wallRotation == Tile::LEFT) {
			SDL_RenderCopyEx(renderer, textures[wallType], &srcRect, &wallRect, (double)wallRotation, NULL, SDL_FLIP_NONE);
		} else {
			SDL_RenderCopyEx(renderer, textures[wallType], &srcRect, &wallRect, (double)wallRotation, NULL, SDL_FLIP_NONE);
		}
	}

	//Draw enemies
	for (int i = 0; i < enemies.size(); i++) {

		double enemyX = enemies[i].pos.x - offsetX;
		double enemyY = enemies[i].pos.y - offsetY;
		bool hitEffect = enemies[i].hitEffect;
		int enemyType = enemies[i].type;
		int enemySize = enemies[i].enemySize;

		SDL_Rect enemyRect = CreateRect(enemyX - enemySize/2, enemyY - enemySize/2, enemySize, enemySize);

		//Draw enemy (super slow)
		//if (enemyType == 0 && hitEffect) {
		//	filledCircleRGBA(renderer, enemyX, enemyY, playerSize, 255, 255, 51, 255);
		//} else if (enemyType == 1 && hitEffect){
		//	filledCircleRGBA(renderer, enemyX, enemyY, playerSize, 255, 255, 255, 255);
		//} else if (enemyType == 1){
		//	filledCircleRGBA(renderer, enemyX, enemyY, playerSize, 50, 50, 50, 255);
		//} else {
		//	filledCircleRGBA(renderer, enemyX, enemyY, playerSize, 255, 0, 0, 255);
		//}
		//aacircleRGBA(renderer, enemyX, enemyY, playerSize, 0, 0, 0, 255);

		//Draw enemy
		if (enemyType == 0 && hitEffect) {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYMELEE_DAMAGED], &srcRect, &enemyRect);
		} else if (enemyType == 1 && hitEffect) {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYSHOOTER_DAMAGED], &srcRect, &enemyRect);
		} else if (enemyType == 1) {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYSHOOTER], &srcRect, &enemyRect);
		} else {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYMELEE], &srcRect, &enemyRect);
		}

		//Draw extra stuff for the enemies that shoot
		if (enemyType == 1) {
			//Draw aim line
			int enemyAimX = enemies[i].aimAngle.x - offsetX;
			int enemyAimY = enemies[i].aimAngle.y - offsetY;

			//aalineRGBA(renderer, enemyX, enemyY, enemyAimX, enemyAimY, 255, 0, 0, 255);

			//Draw projectiles of current enemy (deprecated)
			/*for (int j = 0; j < enemies[i].projectiles.size(); j++) {
				double bulletX = enemies[i].projectiles[j].x - offsetX;
				double bulletY = enemies[i].projectiles[j].y - offsetY;
				int bulletSize = 64;
				double angle = std::atan2(enemies[i].projectiles[j].shootDirection.x, enemies[i].projectiles[j].shootDirection.y);
				double degrees = 180 * angle / M_PI;
				int angleInDegrees = -(static_cast<int>(360 + trunc(degrees)) % 360) + 180;

				SDL_Rect bulletRect;
				bulletRect.w = bulletSize;
				bulletRect.h = bulletSize;
				bulletRect.x = bulletX - bulletSize / 2;
				bulletRect.y = bulletY - bulletSize / 2;
				SDL_RenderCopyEx(renderer, textures[Tile::ENEMYBULLET], &srcRect, &bulletRect, (double)angleInDegrees, NULL, SDL_FLIP_NONE);
			}*/
		}
	}

	//Draw enemy bullets
	for (int j = 0; j < enemyBullets.size(); j++) {

		double bulletX = enemyBullets[j].x - offsetX;
		double bulletY = enemyBullets[j].y - offsetY;
		int bulletSize = enemyBullets[j].bulletSize;

		double angle = std::atan2(enemyBullets[j].shootDirection.y, enemyBullets[j].shootDirection.x);
		double degrees = (180 * angle / M_PI) + 90;

		SDL_Rect bulletRect = CreateRect(bulletX - bulletSize / 2, bulletY - bulletSize / 2, bulletSize, bulletSize);

		SDL_RenderCopyEx(renderer, textures[Tile::ENEMYBULLET], &srcRect, &bulletRect, degrees, NULL, SDL_FLIP_NONE);
	}

	//Draw projectiles
	for (int i = 0; i < gPlayer->projectiles.size(); i++) {

		double bulletX = gPlayer->projectiles[i].x - offsetX;
		double bulletY = gPlayer->projectiles[i].y - offsetY;
		int bulletSize = gPlayer->projectiles[i].bulletSize;

		double angle = std::atan2(gPlayer->projectiles[i].shootDirection.y, gPlayer->projectiles[i].shootDirection.x); //angle in radians
		double degrees = (180 * angle / M_PI) + 90; //convert to degrees (offset by 90 degrees)

		SDL_Rect bulletRect = CreateRect(bulletX - bulletSize / 2, bulletY - bulletSize / 2, bulletSize, bulletSize);
		SDL_RenderCopyEx(renderer, textures[Tile::PLAYERBULLET], &srcRect, &bulletRect, degrees, NULL, SDL_FLIP_NONE);
	}

	//Draw line of sight
	for (int s = 0; s < shadowPoints.size(); s++) {
		int shadowX = shadowPoints[s].first.x - offsetX;
		int shadowY = shadowPoints[s].first.y - offsetY;
		SDL_Rect shadowRect = CreateRect(shadowX, shadowY, 24, 24);
		//SDL_RenderCopy(renderer, textures[Tile::PLAYER], &srcRect, &shadowRect);
		if (s == shadowPoints.size()-1) {
			lineRGBA(renderer, shadowX, shadowY, shadowPoints[s - s].first.x - offsetX, shadowPoints[s - s].first.y - offsetY, 255, 0, 0, 255);
			//filledTrigonRGBA(renderer, gPlayer->pos.x - offsetX, gPlayer->pos.y - offsetY, shadowX, shadowY, shadowPoints[s - s].first.x - offsetX, shadowPoints[s - s].first.y - offsetY, 255, 255, 255, 100);
		} else {
			lineRGBA(renderer, shadowX, shadowY, shadowPoints[s +1].first.x - offsetX, shadowPoints[s+1].first.y - offsetY, 255, 0, 0, 255);
			//filledTrigonRGBA(renderer, gPlayer->pos.x - offsetX, gPlayer->pos.y - offsetY, shadowX, shadowY, shadowPoints[s + 1].first.x - offsetX, shadowPoints[s + 1].first.y - offsetY, 255, 255, 255, 100);
		}
		gShadows->shadowPoints.clear();
	}

	//Draw player
	SDL_Rect playerRect = CreateRect(playerX - playerSize / 2, playerY - playerSize / 2, playerSize, playerSize);
	if (gPlayer->invincible) {
		SDL_SetTextureAlphaMod(textures[Tile::PLAYER], 150);
		SDL_SetTextureColorMod(textures[Tile::PLAYER], 255, 1, 1);
	} else {
		SDL_SetTextureAlphaMod(textures[Tile::PLAYER], 255);
		SDL_SetTextureColorMod(textures[Tile::PLAYER], 255, 255, 255);
	}
	SDL_RenderCopy(renderer, textures[Tile::PLAYER], &srcRect, &playerRect);

	//Draw player's aim line
	aalineRGBA(renderer, playerX, playerY, aimAngleX, aimAngleY, 0, 0, 0, 255);

	//Draw bounding boxes used for collision detection testing (enable/disable in main file)
	if (gCollision->renderBounds) {
		std::vector<std::pair<SDL_Rect, bool>> boxes = gCollision->boxes;

		for (int b = 0; b < boxes.size(); b++) {

			if (boxes.size() < 1000) {
				double boxX1 = boxes[b].first.x - offsetX;
				double boxY1 = boxes[b].first.y - offsetY;
				int boxX2 = (boxes[b].first.x + boxes[b].first.w) - offsetX;
				int boxY2 = (boxes[b].first.y + boxes[b].first.h) - offsetY;

				if (!boxes[b].second) {
					rectangleRGBA(renderer, boxX1, boxY1, boxX2, boxY2, 255, 255, 0, 255);
				} else {
					rectangleRGBA(renderer, boxX1, boxY1, boxX2, boxY2, 255, 0, 0, 255);
				}

			} else {
				boxCounter = 0;
				gCollision->boxes.clear();
			}
			boxCounter++;
		}
	}

	//Draw text
	RenderText("Enemies remaining: " + std::to_string(numEnemies), (gCamera->windowX / 2) - (250 / 2), gCamera->windowY - 50, 250, 50, { 255, 0, 0 });
	RenderText("HP: " + std::to_string(gPlayer->currentHP), 100, gCamera->windowY - 50, 70, 50, { 255, 0, 0 });

	//Draw cursor (unresponsive, use SDL's CreateColorCursor instead)
	//filledCircleRGBA(renderer, aimPosX, aimPosY, 10, 255, 255, 0, 255);
	//int crosshairSize = 40;
	//SDL_Rect crosshairRect = CreateRect(aimPosX - crosshairSize / 2, aimPosY - crosshairSize / 2, crosshairSize, crosshairSize);
	//SDL_RenderCopy(renderer, textures[Tile::CROSSHAIR], &srcRect, &crosshairRect);

	//Present everything on screen and set background color
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); //This color is selected to replace everything on screen
	SDL_RenderClear(renderer); //Everything is cleared except the selected background color
}

void Graphics::RenderText(std::string text, int x, int y, int w, int h, SDL_Color color) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRect;
	textRect.x = x;
	textRect.y = y;
	textRect.w = w;
	textRect.h = h;
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

Graphics::~Graphics() {
	TTF_CloseFont(font);
}