#include "2ds_tile.h"

Tile::Tile(tileType typeOfTile, int tileHeight, int tileWidth, Vector2 position, Tile::tileRotation rotation) {
	this->type = typeOfTile;
	this->tileHeight = tileHeight;
	this->tileWidth = tileWidth;
	this->position = position;
	this->rotation = rotation;
	this->isCollidable = false;
	this->isVisible = true;
	for (int i = 0; i < this->collidableTiles.size(); i++) {
		if (this->type == collidableTiles[i]) {
			this->isCollidable = true;
		}
	}
	for (int i = 0; i < this->invisibleTiles.size(); i++) {
		if (this->type == invisibleTiles[i]) {
			this->isVisible = false;
		}
	}
}

SDL_Rect Tile::GetRect() {
	SDL_Rect rect;
	rect.w = this->tileWidth;
	rect.h = this->tileHeight;
	rect.x = this->position.x;
	rect.y = this->position.y;
	return rect;
}

Vector2 Tile::GetCenter() {
	int centerX = position.x + tileWidth / 2;
	int centerY = position.y + tileHeight / 2;
	return Vector2(centerX, centerY);
}

Tile::~Tile() {

}