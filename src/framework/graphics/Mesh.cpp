#include "Mesh.h"

#include <cstddef>

#include "framework/graphics/GLContext.h"

namespace Framework
{
	Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned> indices, Shader* shader)
		: _verts(verts)
		, _indices(indices)
		, _pShader(shader)
	{
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _verts.size(), &_verts[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

		int sa = sizeof(_verts[0]) * _verts.size();
		int sv = _verts.size();
		int s = sizeof(Vertex);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		
		// normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));

		// tex coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));

		// color
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));
		

		//_pShader->setUniform("u_MVP", glm::mat4(1.f), false);

		//GLenum err = glGetError();
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
	}

	void Mesh::draw() const
	{
		GLenum err = glGetError();
		_pShader->use();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glUseProgram(0);
		err = glGetError();
	}

}