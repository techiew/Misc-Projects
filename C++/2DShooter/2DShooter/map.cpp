#include "map.h"

// DISCLAIMER: The map generation code is horribly messy, but atleast it works.

Map::Map(Vector2 origin, int tileHeight, int tileWidth) {
	this->origin = origin;
	this->tileHeight = 64;
	this->tileWidth = 64;
	this->spawnpoint = Vector2(0, 0);
	this->currentLevelWalls.clear();
	this->currentLevelTiles.clear();
	this->numOfTiles = 0;
	this->rooms.clear();
	this->currentLevelEnemySpawns.clear();
}

void Map::CreateTile(Tile::tileType type, int height, int width, Vector2 pos, Tile::tileRotation rotation, Vector2 offset) {
	Vector2 position(pos.x + offset.x, pos.y + offset.y);
	Tile tile(type, height, width, position, rotation);
	if (type == Tile::ENEMYSPAWN) {
		currentLevelEnemySpawns.push_back(tile);
	}
	if (tile.isCollidable) {
		currentLevelWalls.push_back(tile);
	} else {
		currentLevelTiles.push_back(tile);
	}
	numOfTiles++;
}

void Map::RemoveTile(Vector2 pos, Vector2 offset) {
	Vector2 position(pos.x + offset.x, pos.y + offset.y);
	for (int i = 0; i < currentLevelWalls.size(); i++) {
		if (currentLevelWalls[i].position.x == position.x && currentLevelWalls[i].position.y == position.y) {
			currentLevelWalls.erase(currentLevelWalls.begin() + i);
		}
	}
}

std::vector<Enemy> Map::SpawnEnemies() {
	std::vector<Enemy> enemies;
	for (int i = 0; i < currentLevelEnemySpawns.size(); i++) {
		Enemy enemy(Vector2(currentLevelEnemySpawns[i].position.x + (tileWidth / 2), currentLevelEnemySpawns[i].position.y + (tileHeight / 2)), GetRandInt(0, 1));
		enemies.push_back(enemy);
	}
	currentLevelEnemies = enemies;
	return enemies;
}

bool Map::RoomCollision(Vector2 origin0, Vector2 bounds0) {
	for (int i = 0; i < rooms.size(); i++) {
		Vector2 origin1 = rooms[i].origin;
		Vector2 bounds1 = rooms[i].bounds;
		if (origin0.x <= origin1.x + bounds1.y &&
			origin0.x + bounds0.y >= origin1.x &&
			origin0.y <= origin1.y + bounds1.x &&
			origin0.y + bounds0.x >= origin1.y) {
			return true;
		}
	}
	return false;
}

void Map::CreateRoom(Vector2 origin, int height, int width, bool isChild, int dir) {
	Vector2 bounds((height + 2)*tileHeight, (width + 2)*tileWidth);
	Room newRoom(origin, bounds);
	// Create room floor
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			CreateTile(Tile::FLOOR, tileHeight, tileWidth, Vector2(tileWidth * i, tileHeight * j), Tile::DOWN, origin);
			int createSpawn = GetRandInt(1, 10);
			if (createSpawn == 10 && rooms.size() != 0) {
				CreateTile(Tile::ENEMYSPAWN, tileHeight, tileWidth, Vector2(tileWidth * i, tileHeight * j), Tile::DOWN, origin);
			}
		}
	}
	// Create room walls
	// North
	for (int n = 0; n < width; n++) {
		CreateTile(Tile::WALL, tileHeight, tileWidth, Vector2(tileWidth * n, -tileHeight), Tile::DOWN, origin);
	}

	// West
	for (int w = 0; w < height; w++) {
		CreateTile(Tile::WALL, tileHeight, tileWidth, Vector2(-tileWidth, tileHeight * w), Tile::RIGHT, origin);
	}

	// South
	for (int s = 0; s < width; s++) {
		CreateTile(Tile::WALL, tileHeight, tileWidth, Vector2(tileWidth * s, tileHeight * height), Tile::UP, origin);
	}

	// East
	for (int e = 0; e < height; e++) {
		CreateTile(Tile::WALL, tileHeight, tileWidth, Vector2(tileWidth * width, tileHeight * e), Tile::LEFT, origin);
	}
	
	// Create corners, same order as above
	CreateTile(Tile::WALLCORNER, tileHeight, tileWidth, Vector2(-tileWidth, -tileHeight), Tile::DOWN, origin);
	CreateTile(Tile::WALLCORNER, tileHeight, tileWidth, Vector2(-tileWidth, tileHeight * height), Tile::RIGHT, origin);
	CreateTile(Tile::WALLCORNER, tileHeight, tileWidth, Vector2(tileWidth * width, tileHeight * height), Tile::UP, origin);
	CreateTile(Tile::WALLCORNER, tileHeight, tileWidth, Vector2(tileWidth * width, -tileHeight), Tile::LEFT, origin);

	// Create exits
	int exits;
	exits = GetRandInt(2, 4);
	bool north = false, west = false, south = false, east = false;
	bool hasChild = false;

	if (isChild) {
		hasChild = true;
		exits = GetRandInt(0, 3);
		if (dir == 1) { north = true; } 
		else if (dir == 2) { west = true; } 
		else if (dir == 3) { south = true; } 
		else if (dir == 4) { east = true; }
	}

	for (int i = 0; i < exits; i++) {
		int minHeight = (height / 2) / 2;
		int maxHeight = (height / 2) + (height / 2) / 2;
		int minWidth = (width / 2) / 2;
		int maxWidth = (width / 2) + (width / 2) / 2;
		int exitHeight = GetRandInt(minHeight, maxHeight);
		int exitWidth = GetRandInt(minWidth, maxWidth);
		int exitSide = GetRandInt(1, 4);

		Vector2 exitPos1;
		Vector2 exitPos2;
		Vector2 exitPos1Global;
		Vector2 exitPos2Global;

		switch (exitSide) {
		case 1:
			// North
			if (north == true) { i--; break; }
			if (exitWidth == minWidth){
				exitPos1.Set(tileWidth * exitWidth, -tileHeight);
				exitPos2.Set(tileWidth * (exitWidth + 1), -tileHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitWidth == maxWidth) {
				exitPos1.Set(tileWidth * (exitWidth - 1), -tileHeight);
				exitPos2.Set(tileWidth * exitWidth, -tileHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(tileWidth * exitWidth, -tileHeight);
				exitPos2.Set(tileWidth * (exitWidth + 1), -tileHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos2, Tile::DOWN, origin);

			// Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x - tileWidth, exitPos1.y), origin);
			RemoveTile(Vector2(exitPos2.x + tileWidth, exitPos2.y), origin);
			CreateTile(Tile::WALLEDGE_R, tileHeight, tileWidth, Vector2(exitPos1.x - tileWidth, exitPos1.y), Tile::DOWN, origin);
			CreateTile(Tile::WALLEDGE_L, tileHeight, tileWidth, Vector2(exitPos2.x + tileWidth, exitPos2.y), Tile::DOWN, origin);

			/*if (!isChild) {
				CreateRoom(exitPos1, 5, 5, true, 1);
			}*/

			north = true;
			break;

		case 2:
			// West
			if (west == true) { i--; break; }
			if (exitHeight == minHeight) {
				exitPos1.Set(-tileWidth, tileHeight * (exitHeight - 1));
				exitPos2.Set(-tileWidth, tileHeight * exitHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitHeight == maxHeight) {
				exitPos1.Set(-tileWidth, tileHeight * exitHeight);
				exitPos2.Set(-tileWidth, tileHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(-tileWidth, tileHeight * exitHeight);
				exitPos2.Set(-tileWidth, tileHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);
			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos2, Tile::DOWN, origin);

			// Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x, exitPos1.y - tileHeight), origin);
			RemoveTile(Vector2(exitPos2.x, exitPos2.y + tileHeight), origin);
			CreateTile(Tile::WALLEDGE_L, tileHeight, tileWidth, Vector2(exitPos1.x, exitPos1.y - tileWidth), Tile::RIGHT, origin);
			CreateTile(Tile::WALLEDGE_R, tileHeight, tileWidth, Vector2(exitPos2.x, exitPos2.y + tileWidth), Tile::RIGHT, origin);

			west = true;
			break;

		case 3:
			// South
			if (south == true) { i--; break; }
			if (exitWidth == minWidth) {
				exitPos1.Set(tileWidth * exitWidth, tileHeight * height);
				exitPos2.Set(tileWidth * (exitWidth + 1), tileHeight * height);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitWidth == maxWidth) {
				exitPos1.Set(tileWidth * (exitWidth - 1), tileHeight * height);
				exitPos2.Set(tileWidth * exitWidth, tileHeight * height);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(tileWidth * exitWidth, tileHeight * height);
				exitPos2.Set(tileWidth * (exitWidth + 1), tileHeight * height);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos2, Tile::DOWN, origin);

			// Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x - tileWidth, exitPos1.y), origin);
			RemoveTile(Vector2(exitPos2.x + tileWidth, exitPos2.y), origin);
			CreateTile(Tile::WALLEDGE_L, tileHeight, tileWidth, Vector2(exitPos1.x - tileWidth, exitPos1.y), Tile::UP, origin);
			CreateTile(Tile::WALLEDGE_R, tileHeight, tileWidth, Vector2(exitPos2.x + tileWidth, exitPos2.y), Tile::UP, origin);

			south = true;
			break;

		case 4:
			// East
			if (east == true) { i--; break; }
			if (exitHeight == minHeight) {
				exitPos1.Set(tileWidth * width, tileHeight * exitHeight);
				exitPos2.Set(tileWidth * width, tileHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitHeight == maxHeight) {
				exitPos1.Set(tileWidth * width, tileHeight * (exitHeight - 1));
				exitPos2.Set(tileWidth * width, tileHeight * exitHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(tileWidth * width, tileHeight * exitHeight);
				exitPos2.Set(tileWidth * width, tileHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::FLOOR, tileHeight, tileWidth, exitPos2, Tile::DOWN, origin);

			// Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x, exitPos1.y - tileHeight), origin);
			RemoveTile(Vector2(exitPos2.x, exitPos2.y + tileHeight), origin);
			CreateTile(Tile::WALLEDGE_R, tileHeight, tileWidth, Vector2(exitPos1.x, exitPos1.y - tileWidth), Tile::LEFT, origin);
			CreateTile(Tile::WALLEDGE_L, tileHeight, tileWidth, Vector2(exitPos2.x, exitPos2.y + tileWidth), Tile::LEFT, origin);

			east = true;
			break;
		}
	}
	rooms.push_back(newRoom);
}

std::vector<Tile> Map::CreateRandomMap() {
	rooms.clear();
	currentLevelTiles.clear();
	currentLevelWalls.clear();
	currentLevelEnemySpawns.clear();
	currentLevelEnemies.clear();
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	// Create spawn room
	int spawnroomWidth = 5;
	int spawnroomHeight = 5;
	Vector2 origin(0, 0);
	CreateRoom(origin, spawnroomWidth, spawnroomHeight, false, 0);
	spawnpoint = Vector2(tileWidth * (spawnroomWidth / 2), tileHeight * (spawnroomHeight / 2));

	int maxRoomSize = 15;
	int minRoomSize = 6;
	int numOfRooms = 10;

	Vector2 dirVector(0, 0);
	for (int i = 0; i < numOfRooms; i++) {

	retry:
		int dir = GetRandInt(1, 8);
		int roomSizeWidth = GetRandInt(minRoomSize, maxRoomSize);
		int roomSizeHeight = GetRandInt(minRoomSize, maxRoomSize);
		int index = GetRandInt(0, rooms.size()-1);
		int tries = 0;
		bool valid = true;

		switch (dir) {
		case 1:
			dirVector.Set(0, -1); // Up
			break;
		case 2:
			dirVector.Set(1, 0); // Right
			break;
		case 3:
			dirVector.Set(0, 1); // Down
			break;
		case 4:
			dirVector.Set(-1, 0); // Left
			break;
		case 5:
			dirVector.Set(1, -1); // Upper right
			break;
		case 6:
			dirVector.Set(-1, -1); // Upper left
			break;
		case 7:
			dirVector.Set(1, 1); // Lower right
			break;
		case 8:
			dirVector.Set(-1, 1); // Lower left
			break;
		}

		//std::cout << "Parent room is " << index << std::endl;
		Vector2 roomPos(rooms[index].origin.x + (dirVector.x * (tileWidth * (minRoomSize+3))), rooms[index].origin.y + (dirVector.y * (tileHeight * (minRoomSize+3))));
		Vector2 bounds((roomSizeHeight+2)*tileHeight, (roomSizeWidth+2)*tileWidth);
		//std::cout << "Attempting to place room at: " << roomPos.x << ", " << roomPos.y << std::endl;
		//std::cout << "Height: " << bounds.x << ", Width: " << bounds.y << std::endl;

		int moveX = ChooseInt(std::vector<int> { 1, -1, 2, -2 });
		int moveY = ChooseInt(std::vector<int> { 1, -1, 2, -2 });
		
		int maxBranches = 2;

		if (RoomCollision(roomPos, bounds)) {
			valid = false;
			do {
				//std::cout << "Collision with " << rooms[index].origin.x << " - " << rooms[index].origin.y << std::endl;
				//std::cout << "Current pos: " << roomPos.x << ", " << roomPos.y << std::endl;
				if (tries % 10 == 0) {
					moveX = ChooseInt(std::vector<int> { 1, -1, 2, -2 });
					moveY = ChooseInt(std::vector<int> { 1, -1, 2, -2 });
				}
				roomPos.Set(
					roomPos.x + (moveX * tileWidth),
					roomPos.y + (moveY * tileHeight)
				);
				//std::cout << "Moved to " << roomPos.x << ", " << roomPos.y << std::endl;
				tries++;
				if (tries >= 30) goto retry;
			} while (RoomCollision(roomPos, bounds));
			valid = true;
		}

		if (valid) {
			//std::cout << "Placing room at: " << roomPos.x << " - " << roomPos.y << std::endl;
			CreateRoom(roomPos, roomSizeHeight, roomSizeWidth, false, 0);
		}
	}

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedTime = end - start;
	std::cout << "Generated map in: " << elapsedTime.count() << " seconds, " << numOfTiles << " tiles, " << currentLevelEnemySpawns.size() << " enemies" << std::endl;

	return currentLevelTiles;
}