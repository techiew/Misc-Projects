#pragma once

#include <vector>
#include <iostream>
#include <ctime>
#include <chrono>
#include <functional>
#include "2ds_vector2.h"
#include "2ds_rand.h"
#include "2ds_room.h"
#include "2ds_tile.h"
#include "2ds_enemy.h"

class Map {
public:
	int cellHeight;
	int cellWidth;
	int numOfTiles;
	bool isAIEnabled;
	Vector2 origin;
	Vector2 spawnpoint;
	std::vector<Tile> currentLevelTiles;
	std::vector<Tile> currentLevelWalls;
	std::vector<Tile> currentLevelEnemySpawns;
	std::vector<Enemy> currentLevelEnemies;
	std::vector<Room> rooms;

	std::vector<Tile> CreateRandomMap();
	void CreateRoom(Vector2 origin, int height, int width, bool isChild, int dir);
	void CreateTile(Tile::tileType type, int height, int width, Vector2 pos, Tile::tileRotation rotation, Vector2 offset);
	void RemoveTile(Vector2 pos, Vector2 offset);
	std::vector<Enemy> SpawnEnemies();
	bool RoomCollision(Vector2 origin0, Vector2 bounds0);
	Map(Vector2 origin, int cellHeight, int cellWidth);
	~Map();
};