#pragma once

#include <string>
#include <SDL.h>
#include <vector>
#include <map>
#include "2ds_tile.h"

class Textures {
public:
	std::map<int, std::string> textureMap;
	void AddTexture(int index, std::string filepath);
	SDL_Texture* LoadTexture(SDL_Renderer* renderer, std::string filepath);
	std::map<int, SDL_Texture*> PreLoadTextures(SDL_Renderer* renderer);
	Textures();
	~Textures();
};