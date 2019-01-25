#include "2ds_map.h"

Map::Map(Vector2 origin, int cellHeight, int cellWidth) {
	this->origin = origin;
	/*this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;*/
	this->cellHeight = 64;
	this->cellWidth = 64;
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
		Enemy enemy(Vector2(currentLevelEnemySpawns[i].position.x + (cellWidth / 2), currentLevelEnemySpawns[i].position.y + (cellHeight / 2)), GetRandInt(0, 1));
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
	Vector2 bounds((height + 2)*cellHeight, (width + 2)*cellWidth);
	Room newRoom(origin, bounds);
	//Create room floor
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, Vector2(cellWidth * i, cellHeight * j), Tile::DOWN, origin);
			int createSpawn = GetRandInt(1, 10);
			if (createSpawn == 10 && rooms.size() != 0) {
				CreateTile(Tile::ENEMYSPAWN, cellHeight, cellWidth, Vector2(cellWidth * i, cellHeight * j), Tile::DOWN, origin);
			}
		}
	}
	//Create room walls
	//North
	for (int n = 0; n < width; n++) {
		CreateTile(Tile::BLUEWALL, cellHeight, cellWidth, Vector2(cellWidth * n, -cellHeight), Tile::DOWN, origin);
	}

	//West
	for (int w = 0; w < height; w++) {
		CreateTile(Tile::BLUEWALL, cellHeight, cellWidth, Vector2(-cellWidth, cellHeight * w), Tile::RIGHT, origin);
	}

	//South
	for (int s = 0; s < width; s++) {
		CreateTile(Tile::BLUEWALL, cellHeight, cellWidth, Vector2(cellWidth * s, cellHeight * height), Tile::UP, origin);
	}

	//East
	for (int e = 0; e < height; e++) {
		CreateTile(Tile::BLUEWALL, cellHeight, cellWidth, Vector2(cellWidth * width, cellHeight * e), Tile::LEFT, origin);
	}
	
	//Create corners, same order as above
	CreateTile(Tile::BLUEWALLCORNER, cellHeight, cellWidth, Vector2(-cellWidth, -cellHeight), Tile::DOWN, origin);
	CreateTile(Tile::BLUEWALLCORNER, cellHeight, cellWidth, Vector2(-cellWidth, cellHeight * height), Tile::RIGHT, origin);
	CreateTile(Tile::BLUEWALLCORNER, cellHeight, cellWidth, Vector2(cellWidth * width, cellHeight * height), Tile::UP, origin);
	CreateTile(Tile::BLUEWALLCORNER, cellHeight, cellWidth, Vector2(cellWidth * width, -cellHeight), Tile::LEFT, origin);

	//Create exits
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
			//North
			if (north == true) { i--; break; }
			if (exitWidth == minWidth){
				exitPos1.Set(cellWidth * exitWidth, -cellHeight);
				exitPos2.Set(cellWidth * (exitWidth + 1), -cellHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitWidth == maxWidth) {
				exitPos1.Set(cellWidth * (exitWidth - 1), -cellHeight);
				exitPos2.Set(cellWidth * exitWidth, -cellHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(cellWidth * exitWidth, -cellHeight);
				exitPos2.Set(cellWidth * (exitWidth + 1), -cellHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos2, Tile::DOWN, origin);

			//Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x - cellWidth, exitPos1.y), origin);
			RemoveTile(Vector2(exitPos2.x + cellWidth, exitPos2.y), origin);
			CreateTile(Tile::BLUEWALLEDGE_R, cellHeight, cellWidth, Vector2(exitPos1.x - cellWidth, exitPos1.y), Tile::DOWN, origin);
			CreateTile(Tile::BLUEWALLEDGE_L, cellHeight, cellWidth, Vector2(exitPos2.x + cellWidth, exitPos2.y), Tile::DOWN, origin);

			/*if (!isChild) {
				CreateRoom(exitPos1, 5, 5, true, 1);
			}*/

			north = true;
			break;

		case 2:
			//West
			if (west == true) { i--; break; }
			if (exitHeight == minHeight) {
				exitPos1.Set(-cellWidth, cellHeight * (exitHeight - 1));
				exitPos2.Set(-cellWidth, cellHeight * exitHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitHeight == maxHeight) {
				exitPos1.Set(-cellWidth, cellHeight * exitHeight);
				exitPos2.Set(-cellWidth, cellHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(-cellWidth, cellHeight * exitHeight);
				exitPos2.Set(-cellWidth, cellHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);
			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos2, Tile::DOWN, origin);

			//Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x, exitPos1.y - cellHeight), origin);
			RemoveTile(Vector2(exitPos2.x, exitPos2.y + cellHeight), origin);
			CreateTile(Tile::BLUEWALLEDGE_L, cellHeight, cellWidth, Vector2(exitPos1.x, exitPos1.y - cellWidth), Tile::RIGHT, origin);
			CreateTile(Tile::BLUEWALLEDGE_R, cellHeight, cellWidth, Vector2(exitPos2.x, exitPos2.y + cellWidth), Tile::RIGHT, origin);

			west = true;
			break;

		case 3:
			//South
			if (south == true) { i--; break; }
			if (exitWidth == minWidth) {
				exitPos1.Set(cellWidth * exitWidth, cellHeight * height);
				exitPos2.Set(cellWidth * (exitWidth + 1), cellHeight * height);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitWidth == maxWidth) {
				exitPos1.Set(cellWidth * (exitWidth - 1), cellHeight * height);
				exitPos2.Set(cellWidth * exitWidth, cellHeight * height);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(cellWidth * exitWidth, cellHeight * height);
				exitPos2.Set(cellWidth * (exitWidth + 1), cellHeight * height);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos2, Tile::DOWN, origin);

			//Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x - cellWidth, exitPos1.y), origin);
			RemoveTile(Vector2(exitPos2.x + cellWidth, exitPos2.y), origin);
			CreateTile(Tile::BLUEWALLEDGE_L, cellHeight, cellWidth, Vector2(exitPos1.x - cellWidth, exitPos1.y), Tile::UP, origin);
			CreateTile(Tile::BLUEWALLEDGE_R, cellHeight, cellWidth, Vector2(exitPos2.x + cellWidth, exitPos2.y), Tile::UP, origin);

			south = true;
			break;

		case 4:
			//East
			if (east == true) { i--; break; }
			if (exitHeight == minHeight) {
				exitPos1.Set(cellWidth * width, cellHeight * exitHeight);
				exitPos2.Set(cellWidth * width, cellHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else if (exitHeight == maxHeight) {
				exitPos1.Set(cellWidth * width, cellHeight * (exitHeight - 1));
				exitPos2.Set(cellWidth * width, cellHeight * exitHeight);
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			} else {
				exitPos1.Set(cellWidth * width, cellHeight * exitHeight);
				exitPos2.Set(cellWidth * width, cellHeight * (exitHeight + 1));
				exitPos1Global.Set(exitPos1.x + origin.x, exitPos1.y + origin.y);
				exitPos2Global.Set(exitPos2.x + origin.x, exitPos2.y + origin.y);

			}
			RemoveTile(exitPos1, origin);
			RemoveTile(exitPos2, origin);
			newRoom.AddExit(exitPos1Global);
			newRoom.AddExit(exitPos2Global);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos1, Tile::DOWN, origin);
			//CreateTile(Tile::GREYFLOOR, cellHeight, cellWidth, exitPos2, Tile::DOWN, origin);

			//Replace tiles next to exits with edges (different texture)
			RemoveTile(Vector2(exitPos1.x, exitPos1.y - cellHeight), origin);
			RemoveTile(Vector2(exitPos2.x, exitPos2.y + cellHeight), origin);
			CreateTile(Tile::BLUEWALLEDGE_R, cellHeight, cellWidth, Vector2(exitPos1.x, exitPos1.y - cellWidth), Tile::LEFT, origin);
			CreateTile(Tile::BLUEWALLEDGE_L, cellHeight, cellWidth, Vector2(exitPos2.x, exitPos2.y + cellWidth), Tile::LEFT, origin);

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

	//Create spawn room
	int spawnroomWidth = 5;
	int spawnroomHeight = 5;
	Vector2 origin(0, 0);
	CreateRoom(origin, spawnroomWidth, spawnroomHeight, false, 0);
	spawnpoint = Vector2(cellWidth * (spawnroomWidth / 2), cellHeight * (spawnroomHeight / 2));

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
			dirVector.Set(0, -1); //Up
			break;
		case 2:
			dirVector.Set(1, 0); //Right
			break;
		case 3:
			dirVector.Set(0, 1); //Down
			break;
		case 4:
			dirVector.Set(-1, 0); //Left
			break;
		case 5:
			dirVector.Set(1, -1); //Upper right
			break;
		case 6:
			dirVector.Set(-1, -1); //Upper left
			break;
		case 7:
			dirVector.Set(1, 1); //Lower right
			break;
		case 8:
			dirVector.Set(-1, 1); //Lower left
			break;
		}

		//std::cout << "Parent room is " << index << std::endl;
		Vector2 roomPos(rooms[index].origin.x + (dirVector.x * (cellWidth * (minRoomSize+3))), rooms[index].origin.y + (dirVector.y * (cellHeight * (minRoomSize+3))));
		Vector2 bounds((roomSizeHeight+2)*cellHeight, (roomSizeWidth+2)*cellWidth);
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
					roomPos.x + (moveX * cellWidth),
					roomPos.y + (moveY * cellHeight)
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

Map::~Map() {

}