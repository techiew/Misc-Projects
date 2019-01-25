#include "2ds_textures.h"
#include <iostream>

Textures::Textures() {
	AddTexture(Tile::GREYFLOOR, "Assets/tile_greyfloor.bmp");
	AddTexture(Tile::GREYWALL, "Assets/tile_greywall.bmp");
	AddTexture(Tile::GREYWALLCORNER, "Assets/tile_greywall_corner.bmp");
	AddTexture(Tile::ENEMYSPAWN, "Assets/enemyspawn.bmp");
	AddTexture(Tile::ENEMYSHOOTER, "Assets/enemy_slimer.bmp");
	AddTexture(Tile::ENEMYSHOOTER_DAMAGED, "Assets/enemy_shooter_damaged.bmp");
	AddTexture(Tile::ENEMYMELEE, "Assets/enemy_melee.bmp");
	AddTexture(Tile::ENEMYMELEE_DAMAGED, "Assets/enemy_melee_damaged.bmp");
	AddTexture(Tile::PLAYERBULLET, "Assets/player_bullet.bmp");
	AddTexture(Tile::ENEMYBULLET, "Assets/enemy_slimeball.bmp");
	AddTexture(Tile::PLAYER, "Assets/player.bmp");
	AddTexture(Tile::BLUEWALL, "Assets/tile_bluewall.bmp");
	AddTexture(Tile::BLUEWALLSINGLE, "Assets/tile_bluewall_single.bmp");
	AddTexture(Tile::BLUEWALLCORNER, "Assets/tile_bluewall_corner.bmp");
	AddTexture(Tile::BLUEWALLEDGE_R, "Assets/tile_bluewall_edge_r.bmp");
	AddTexture(Tile::BLUEWALLEDGE_L, "Assets/tile_bluewall_edge_l.bmp");
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

Textures::~Textures() {

}