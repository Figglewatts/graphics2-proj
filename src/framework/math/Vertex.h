#ifndef VERTEX_H
#define VERTEX_H
#pragma once

#include <glm/glm.hpp>

namespace Framework
{
	struct Vertex
	{
		glm::tvec3<float> Position;
		glm::tvec3<float> Normal;
		glm::tvec2<float> TexCoord;
		glm::tvec4<float> Color;

		Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv, glm::vec4 col)
			: Position(pos), Normal(norm), TexCoord(uv), Color(col) { }
	};
}

#endif // VERTEX_H