#include "textures.h"
#include <iostream>

Textures::Textures() {
	AddTexture(Tile::ENEMYSPAWN, "Assets/enemyspawn.bmp");
	AddTexture(Tile::ENEMYSHOOTER, "Assets/enemy_slimer.bmp");
	AddTexture(Tile::ENEMYSHOOTER_DAMAGED, "Assets/enemy_slimer_damaged.bmp");
	AddTexture(Tile::ENEMYMELEE, "Assets/enemy_melee.bmp");
	AddTexture(Tile::ENEMYMELEE_DAMAGED, "Assets/enemy_melee_damaged.bmp");
	AddTexture(Tile::PLAYERBULLET, "Assets/player_bullet.bmp");
	AddTexture(Tile::ENEMYBULLET, "Assets/enemy_slimeball.bmp");
	AddTexture(Tile::PLAYER, "Assets/player.bmp");
	AddTexture(Tile::FLOOR, "Assets/tile_floor.bmp");
	AddTexture(Tile::WALL, "Assets/tile_wall.bmp");
	AddTexture(Tile::WALLCORNER, "Assets/tile_wall_corner.bmp");
	AddTexture(Tile::WALLEDGE_R, "Assets/tile_wall_edge_r.bmp");
	AddTexture(Tile::WALLEDGE_L, "Assets/tile_wall_edge_l.bmp");
	AddTexture(Tile::CROSSHAIR, "Assets/texture_crosshair.bmp");
}

void Textures::AddTexture(int index, std::string filepath) {
	textureMap[index] = filepath;
}

SDL_Texture* Textures::LoadTexture(SDL_Renderer* renderer, std::string filepath) {
	SDL_Surface* loadSurface = nullptr;
	SDL_Texture* texture = nullptr;
	loadSurface = SDL_LoadBMP(filepath.c_str());
	if (loadSurface == NULL) {
		std::cout << "Texture failed to load: " << filepath << std::endl;
		return nullptr;
	}
	texture = SDL_CreateTextureFromSurface(renderer, loadSurface);
	SDL_FreeSurface(loadSurface);
	return texture;
}

std::map<int, SDL_Texture*> Textures::PreLoadTextures(SDL_Renderer* renderer) {
	std::map<int, SDL_Texture*> textures;
	for (std::map<int, std::string>::iterator it = textureMap.begin(); it != textureMap.end(); it++) {
		SDL_Texture* texture = LoadTexture(renderer, it->second);
		int index = it->first;
		std::cout << it->second << std::endl;
		textures[index] = texture;
	}
	return textures;
}