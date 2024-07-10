#pragma once
#include <vector>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <ElementBuffer.h>
class BufferManager {
public:
	BufferManager() : m_VBO(nullptr), m_EBO(nullptr){};
	~BufferManager() { Cleanup(); };

	void InitializeBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
		m_VAO.Bind();

		m_VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		m_VAO.AddBuffer(*m_VBO, layout);

		m_EBO = new ElementBuffer(indices.data(), indices.size() * sizeof(unsigned int));

		m_VAO.Unbind();
		m_VBO->Unbind();
		m_EBO->Unbind();
	};
	void Bind() const {
		m_VAO.Bind();
	};
	void Unbind() const {
		m_VAO.Unbind();
	};
	void Draw() const {
		m_VAO.Bind();
		glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, nullptr);
		m_VAO.Unbind();
	};
private:
	VertexArray m_VAO;
	VertexBuffer *m_VBO;
	ElementBuffer *m_EBO;
	
	void Cleanup() {
		delete m_VBO;
		delete m_EBO;
	};
};