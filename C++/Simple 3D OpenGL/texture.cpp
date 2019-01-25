#include "texture.h"

Texture::Texture() 
{
	LoadTexture(".\\res\\texturetest.bmp");
}

void Texture::Bind(unsigned int unit) 
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::LoadTexture(std::string filepath) 
{
	SDL_Surface* loadSurface = nullptr;
	loadSurface = SDL_LoadBMP(filepath.c_str());
	if (loadSurface == NULL) {
		std::cerr << "Texture failed to load: " << filepath << std::endl;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, loadSurface->w, loadSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, loadSurface->pixels);

	SDL_FreeSurface(loadSurface);
}

Texture::~Texture() 
{
	glDeleteTextures(1, &texture);
}