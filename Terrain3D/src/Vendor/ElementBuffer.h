#pragma once

class ElementBuffer {
private:
	unsigned int m_RendererID;
public:
	ElementBuffer(const void* data, unsigned int size);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;
};