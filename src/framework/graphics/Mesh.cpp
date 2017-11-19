#include "Mesh.h"

#include <cstddef>

#include "framework/graphics/GLContext.h"

namespace Framework
{
	Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned> indices, Shader* shader, Texture2D* tex)
		: _verts(verts)
		, _indices(indices)
		, _pShader(shader)
		, _pTexture(tex)
	{
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);
		glGenBuffers(1, &_ebo);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _verts.size(), &_verts[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

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
	}

	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ebo);
		glDeleteVertexArrays(1, &_vao);
	}

	void Mesh::draw(glm::mat4 view, glm::mat4 proj) const
	{
		_pShader->bind();
		_pShader->setUniform("model", _model, false);
		_pShader->setUniform("view", view, false);
		_pShader->setUniform("projection", proj, false);
		_pTexture->bind();
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		_pShader->unbind();
		_pTexture->unbind();
	}

}