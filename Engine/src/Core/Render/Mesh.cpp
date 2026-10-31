#include "spch.h"
#include "Mesh.h"
#include <glad/glad.h>
#include <glfw3.h>

namespace Six {
	SMesh::SMesh(SVertexLayout& layout, std::vector<float>& vertices, std::vector<uint32_t>& indices) {
		m_layout = layout;
		m_vertices = vertices;
		m_indices = indices;
		MeshSetup();
	}

	void SMesh::Destroy() {
		if (m_EBO) { glDeleteBuffers(1, &m_EBO);        m_EBO = 0; }
		if (m_VBO) { glDeleteBuffers(1, &m_VBO);        m_VBO = 0; }
		if (m_VAO) { glDeleteVertexArrays(1, &m_VAO);   m_VAO = 0; }
	}

	void SMesh::MeshSetup() {
		glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);   // ← 注意目标
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			m_indices.size() * sizeof(uint32_t),
			m_indices.data(),
			GL_STATIC_DRAW);

		for (const SVertexElement& e:m_layout.elements)
		{
			glVertexAttribPointer(e.location, e.size, e.type, GL_FALSE, m_layout.stride, (void*)e.offset);
			glEnableVertexAttribArray(e.location);
		}

        glBindVertexArray(0);
	}

	void SMesh::Bind() const{
		glBindVertexArray(m_VAO);
	}
	void SMesh::Unbind() const{
		glBindVertexArray(0);
	}
	void SMesh::Draw() const{
		if (m_indices.size() > 0)
		{
			glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, nullptr);
		}
		else {
			int vertexCount = (int)(m_vertices.size() * sizeof(float) / m_layout.stride);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
	}
}
