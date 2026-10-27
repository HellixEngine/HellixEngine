#include "opengl_buffer.hpp"

namespace hellix::renderer {

    // =========================================================================
    // OPENGL VERTEX BUFFER
    // =========================================================================

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size) {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        if (m_rendererID) {
            glDeleteBuffers(1, &m_rendererID);
        }
    }

    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void* data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    // =========================================================================
    // OPENGL INDEX BUFFER
    // =========================================================================

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
        : m_count(count) {
        glGenBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        if (m_rendererID) {
            glDeleteBuffers(1, &m_rendererID);
        }
    }

    void OpenGLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLIndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}