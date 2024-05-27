#include "GL/glew.h"
#include "ElementBuffer.h"
#include "Window.h"

ElementBuffer::ElementBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

};
ElementBuffer::~ElementBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
};

void ElementBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
};
void ElementBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
};