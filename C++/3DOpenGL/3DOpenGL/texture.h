#pragma once

#include <string>
#include <SDL.h>
#include <iostream>
#include <GL/glew.h>

class Texture {
public:
	GLuint texture;

	Texture();
	void Bind(unsigned int unit);
	void LoadTexture(std::string filepath);
	~Texture();
};