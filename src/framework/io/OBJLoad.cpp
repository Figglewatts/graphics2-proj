#include "OBJLoad.h"

#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "framework/util/Logger.h"
#include "framework/graphics/Mesh.h"

namespace Framework
{
	Mesh* OBJLoad::Load(const std::string & path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str()))
		{
			LOG(LogLevel::FATAL, "Could not load obj: %s", err.c_str());
			throw std::runtime_error(err);
		}

		std::vector<Vertex> verts;
		std::vector<unsigned> indices;

		// map for vertex deduplication
		std::unordered_map<Vertex, uint32_t> uniqueVerts = {};
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex v = {};

				v.Position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};
				if (attrib.normals.size() > 0)
				{
					v.Normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}
				if (attrib.texcoords.size() > 0 && index.texcoord_index != -1)
				{
					v.TexCoord = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}
				v.Color = glm::vec4(1.f);

				if (uniqueVerts.count(v) == 0)
				{
					uniqueVerts[v] = static_cast<uint32_t>(verts.size());
					verts.push_back(v);
				}
				indices.push_back(uniqueVerts[v]);
			}
		}

		return new Mesh(verts, indices);
	}
}
