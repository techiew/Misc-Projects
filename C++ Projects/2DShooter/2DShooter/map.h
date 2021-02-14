#pragma once
#include <vector>
#include <iostream>
#include <ctime>
#include <chrono>
#include <functional>
#include "vector2.h"
#include "rand.h"
#include "room.h"
#include "tile.h"
#include "enemy.h"

class Map {
public:
	int tileHeight;
	int tileWidth;
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
	Map(Vector2 origin, int tileHeight, int tileWidth);
};