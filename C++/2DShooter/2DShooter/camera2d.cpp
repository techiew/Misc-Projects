#include "camera2d.h"

Camera2D::Camera2D(int windowX, int windowY) {
	this->offset = Vector2(0, 0);
	this->windowX = windowX;
	this->windowY = windowY;
}

void Camera2D::SetPlayer(Player* player)
{
	this->player = player;
}

void Camera2D::UpdateCamera(int mouseX, int mouseY) {
	int playerSize = player->playerSize;
	this->offset = Vector2(
		(player->pos.x - this->windowX / 2) + playerSize / 2,
		(player->pos.y - this->windowY / 2) + playerSize / 2
	);

	// This makes the camera move with the cursor (targets the midpoint between player and mouse)
	/*Vector2 midpoint(((player->pos.x + offset.x) + mouseX) / 2, ((player->pos.y + offset.y) + mouseY) / 2);
	this->offset = Vector2(
		(midpoint.x - this->windowX / 2),
		(midpoint.y - this->windowY / 2)
	);*/
}

void Camera2D::UpdateWindowSize(int newX, int newY) {
	this->windowX = newX;
	this->windowY = newY;
}

bool Camera2D::IsTileVisible(Tile tile) { 
	int tileX = tile.position.x;
	int tileY = tile.position.y;
	int d = sqrt(((tileX - player->pos.x) * (tileX - player->pos.x)) + ((tileY - player->pos.y) * (tileY - player->pos.y)));
	if (d < (windowX/2)+128){
		return true;
	}
	return false;
}