#ifndef VERTEX_H
#define VERTEX_H
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Framework
{
	struct Vertex
	{
		glm::tvec3<float> Position;
		glm::tvec3<float> Normal;
		glm::tvec2<float> TexCoord;
		glm::tvec4<float> Color;

		Vertex() {}
		Vertex(glm::vec3 pos, glm::vec3 norm = glm::vec3(0), glm::vec2 uv = glm::vec2(0), 
				glm::vec4 col = glm::vec4(1))
			: Position(pos), Normal(norm), TexCoord(uv), Color(col) { }

		bool operator==(const Vertex& other) const
		{
			return Position == other.Position && Normal == other.Normal
				&& TexCoord == other.TexCoord && Color == other.Color;
		}
	};
}

// from: https://stackoverflow.com/a/19195373
template <class T>
inline void hash_combine(std::size_t & s, const T & v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

// hash function for bind with std::unordered_map
namespace std {
	template<> struct hash<Framework::Vertex> {
		size_t operator()(Framework::Vertex const& vertex) const {
			size_t res = 0;
			hash_combine(res, vertex.Position);
			hash_combine(res, vertex.Normal);
			hash_combine(res, vertex.TexCoord);
			hash_combine(res, vertex.Color);
			return res;
		}
	};
}

#endif // VERTEX_H