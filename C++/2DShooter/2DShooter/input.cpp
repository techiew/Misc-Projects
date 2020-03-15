#include "input.h"

Input::Input(Player* player, Camera2D* camera, Collision* collision, bool* isRunning, bool noclip, SDL_Window* window) {
	this->player = player;
	this->camera = camera;
	this->collision = collision;
	this->isRunning = isRunning;
	this->right = false;
	this->left = false;
	this->up = false;
	this->down = false;
	this->sprint = false;
	this->noclip = noclip;
	this->holdShoot = false;
	this->cooldownCount = 0;
	this->moveX = 0;
	this->moveY = 0;
}

void Input::Tick() {
	while (SDL_PollEvent(&event) != 0) { //While instead of if statement, why? because it works
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.scancode) {
			case(SDL_SCANCODE_W):
				up = true;
				break;
			case(SDL_SCANCODE_A):
				left = true;
				break;
			case(SDL_SCANCODE_S):
				down = true;
				break;
			case(SDL_SCANCODE_D):
				right = true;
				break;
			case(SDL_SCANCODE_LSHIFT):
				sprint = true;
				break;
			}
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.scancode) {
			case(SDL_SCANCODE_W):
				up = false;
				break;
			case(SDL_SCANCODE_A):
				left = false;
				break;
			case(SDL_SCANCODE_S):
				down = false;
				break;
			case(SDL_SCANCODE_D):
				right = false;
				break;
			case(SDL_SCANCODE_LSHIFT):
				sprint = false;
				break;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			switch (event.button.button) {
			case(SDL_BUTTON_LEFT):
				holdShoot = true;
				break;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			switch (event.button.button) {
			case(SDL_BUTTON_LEFT):
				holdShoot = false;
				break;
			}
		}
		else if (event.type == SDL_QUIT) {
			*isRunning = false;
		}
		else if (event.type == SDL_WINDOWEVENT) {
			switch (event.window.event) {
			case SDL_WINDOWEVENT_MAXIMIZED:
				break;
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				sprint, up, down, left, right = false;
				camera->UpdateWindowSize(event.window.data1, event.window.data2);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MOVED:
			case SDL_WINDOWEVENT_TAKE_FOCUS:
			case SDL_WINDOWEVENT_HIDDEN:
				sprint, up, down, left, right = false;
				break;
			}
		}
	}

	double movementPerTick = player->movementPerTick;

	if (holdShoot == true) {
		if (player->shootCooldown == false) {
			player->Shoot();
		}
	}

	if (player->shootCooldown == true) {
		cooldownCount++;
	}
	if (cooldownCount >= 5) {
		player->shootCooldown = false;
		cooldownCount = 0;
	}
	//if (sprint == true) {
		//movementPerTick = movementPerTick * 2;
	//}

	moveX = 0;
	moveY = 0;

	if (up == true) moveY -= 1;
	if (down == true) moveY += 1;
	if (right == true) moveX += 1;
	if (left == true) moveX -= 1;

	if (moveX != 0) {
		if (!collision->CollisionWithWalls(Vector2(player->pos.x + (movementPerTick * moveX), player->pos.y)) || noclip) {
			player->pos.x += (movementPerTick * moveX);
		}
	}
	if (moveY != 0) {
		if (!collision->CollisionWithWalls(Vector2(player->pos.x, player->pos.y + (movementPerTick * moveY))) || noclip) {
			player->pos.y += (movementPerTick * moveY);
		}
	}

	int getMouseX, getMouseY;
	SDL_GetMouseState(&getMouseX, &getMouseY);
	player->SetAimPos(Vector2(getMouseX, getMouseY));
	camera->UpdateCamera(0, 0);
}

Input::~Input() {

}