#include "2ds_camera2d.h"

Camera2D::Camera2D(int windowX, int windowY) {
	this->offset = Vector2(0, 0);
	this->windowX = windowX;
	this->windowY = windowY;
}

void Camera2D::UpdateCamera(int mouseX, int mouseY) {
	int playerSize = gPlayer->playerSize;
	this->offset = Vector2(
		(gPlayer->pos.x - this->windowX / 2) + playerSize / 2,
		(gPlayer->pos.y - this->windowY / 2) + playerSize / 2
	);

	//Camera moves with cursor (midpoint between player and mouse)
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

bool Camera2D::IsTileVisible(Tile tile) { //Check if tile is seen by camera
	int tileX = tile.position.x;
	int tileY = tile.position.y;
	int d = sqrt(((tileX - gPlayer->pos.x) * (tileX - gPlayer->pos.x)) + ((tileY - gPlayer->pos.y) * (tileY - gPlayer->pos.y)));
	if (d < (windowX/2)+128){
		return true;
	}
	return false;
}

Camera2D::~Camera2D() {

}