#pragma once

#include <glm.hpp>

class Vertex
{
public:
	glm::vec3 pos;
	glm::vec4 color;

	Vertex(const glm::vec3& pos, const glm::vec4& color);
};

