#include "Mesh.h"

#include "framework/graphics/GLContext.h"

namespace Framework
{
	Mesh::Mesh(std::vector<glm::vec3> verts, std::vector<unsigned> indices)
		: _verts(verts)
		, _indices(indices)
	{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_verts[0]) * _verts.size(), &_verts.data()->x, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
		glEnableVertexAttribArray(0);
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
	}

	void Mesh::draw() const
	{
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

}