#include "graphics.h"

Graphics::Graphics(Player* player, Camera2D* camera, Collision* collision, EnemyManager* enemyManager,
	Map* map, SDL_Renderer* renderer, std::map<int, SDL_Texture*> textures) {
	this->player = player;
	this->camera = camera;
	this->collision = collision;
	this->enemyManager = enemyManager;
	this->map = map;
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
	this->numEnemies = map->currentLevelEnemySpawns.size();
}

SDL_Rect Graphics::CreateRect(int x, int y, int w, int h) {
	SDL_Rect newRect;
	newRect.x = x;
	newRect.y = y;
	newRect.w = w;
	newRect.h = h;
	return newRect;
}

void Graphics::Render(int numEnemies) {

	std::vector<Tile> tiles = map->currentLevelTiles;
	std::vector<Tile> walls = map->currentLevelWalls;
	std::vector<Projectile> enemyBullets = enemyManager->enemyBullets;
	std::vector<Enemy> enemies = enemyManager->enemies;

	double offsetX = camera->offset.x;
	double offsetY = camera->offset.y;

	int playerX = player->pos.x - offsetX;
	int playerY = player->pos.y - offsetY;
	int playerSize = player->playerSize;
	int aimAngleX = player->aimAngle.x - offsetX;
	int aimAngleY = player->aimAngle.y - offsetY;
	int aimPosX = player->aimPos.x;
	int aimPosY = player->aimPos.y;

	// Draw floor
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

	// Draw walls (drawn after floor, so that the walls are always on top)
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

	// Draw enemies
	for (int i = 0; i < enemies.size(); i++) {

		double enemyX = enemies[i].pos.x - offsetX;
		double enemyY = enemies[i].pos.y - offsetY;
		bool hitEffect = enemies[i].hitEffect;
		int enemyType = enemies[i].type;
		int enemySize = enemies[i].enemySize;

		SDL_Rect enemyRect = CreateRect(enemyX - enemySize/2, enemyY - enemySize/2, enemySize, enemySize);

		// Draw enemy
		if (enemyType == 0 && hitEffect) {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYMELEE_DAMAGED], &srcRect, &enemyRect);
		} else if (enemyType == 1 && hitEffect) {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYSHOOTER_DAMAGED], &srcRect, &enemyRect);
		} else if (enemyType == 1) {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYSHOOTER], &srcRect, &enemyRect);
		} else {
			SDL_RenderCopy(renderer, textures[Tile::ENEMYMELEE], &srcRect, &enemyRect);
		}

	}

	// Draw enemy bullets
	for (int j = 0; j < enemyBullets.size(); j++) {

		double bulletX = enemyBullets[j].x - offsetX;
		double bulletY = enemyBullets[j].y - offsetY;
		int bulletSize = enemyBullets[j].bulletSize;

		double angle = std::atan2(enemyBullets[j].shootDirection.y, enemyBullets[j].shootDirection.x);
		double degrees = (180 * angle / M_PI) + 90;

		SDL_Rect bulletRect = CreateRect(bulletX - bulletSize / 2, bulletY - bulletSize / 2, bulletSize, bulletSize);

		SDL_RenderCopyEx(renderer, textures[Tile::ENEMYBULLET], &srcRect, &bulletRect, degrees, NULL, SDL_FLIP_NONE);
	}

	// Draw projectiles
	for (int i = 0; i < player->projectiles.size(); i++) {

		double bulletX = player->projectiles[i].x - offsetX;
		double bulletY = player->projectiles[i].y - offsetY;
		int bulletSize = player->projectiles[i].bulletSize;

		double angle = std::atan2(player->projectiles[i].shootDirection.y, player->projectiles[i].shootDirection.x); //angle in radians
		double degrees = (180 * angle / M_PI) + 90; //convert to degrees (offset by 90 degrees)

		SDL_Rect bulletRect = CreateRect(bulletX - bulletSize / 2, bulletY - bulletSize / 2, bulletSize, bulletSize);
		SDL_RenderCopyEx(renderer, textures[Tile::PLAYERBULLET], &srcRect, &bulletRect, degrees, NULL, SDL_FLIP_NONE);
	}

	// Draw player
	SDL_Rect playerRect = CreateRect(playerX - playerSize / 2, playerY - playerSize / 2, playerSize, playerSize);
	if (player->invincible) {
		SDL_SetTextureAlphaMod(textures[Tile::PLAYER], 150);
		SDL_SetTextureColorMod(textures[Tile::PLAYER], 255, 1, 1);
	} else {
		SDL_SetTextureAlphaMod(textures[Tile::PLAYER], 255);
		SDL_SetTextureColorMod(textures[Tile::PLAYER], 255, 255, 255);
	}
	SDL_RenderCopy(renderer, textures[Tile::PLAYER], &srcRect, &playerRect);

	// Draw player's aim line
	aalineRGBA(renderer, playerX, playerY, aimAngleX, aimAngleY, 0, 0, 0, 255);

	// Draw bounding boxes used for collision detection testing (enable/disable in main.cpp)
	if (collision->renderBounds) {
		std::vector<std::pair<SDL_Rect, bool>> boxes = collision->boxes;

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
				collision->boxes.clear();
			}
			boxCounter++;
		}
	}

	// Draw text
	RenderText("Enemies remaining: " + std::to_string(numEnemies), (camera->windowX / 2) - (250 / 2), camera->windowY - 50, 250, 50, { 255, 0, 0 });
	RenderText("HP: " + std::to_string(player->currentHP), 100, camera->windowY - 50, 70, 50, { 255, 0, 0 });

	// Draw cursor (unresponsive, use SDL's CreateColorCursor instead)
	//filledCircleRGBA(renderer, aimPosX, aimPosY, 10, 255, 255, 0, 255);
	//int crosshairSize = 40;
	//SDL_Rect crosshairRect = CreateRect(aimPosX - crosshairSize / 2, aimPosY - crosshairSize / 2, crosshairSize, crosshairSize);
	//SDL_RenderCopy(renderer, textures[Tile::CROSSHAIR], &srcRect, &crosshairRect);

	// Present everything on screen and set background color
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); // This color is selected to replace everything on screen
	SDL_RenderClear(renderer); // Everything is cleared except the selected background color
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