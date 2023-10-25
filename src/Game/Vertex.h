#pragma once

struct BaseVertex final
{
	constexpr BaseVertex(glm::vec3 pos) : position(pos), texCoord(glm::vec2{0, 0}), color(glm::vec4{1, 1, 1, 1}) {}
	constexpr BaseVertex(glm::vec3 pos, glm::vec2 uv) : position(pos), texCoord(uv), color(glm::vec4{1, 1, 1, 1}) {}
	constexpr BaseVertex(glm::vec3 pos, glm::vec2 uv, glm::vec4 clr) : position(pos), texCoord(uv), color(clr) {}

	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec4 color;
};