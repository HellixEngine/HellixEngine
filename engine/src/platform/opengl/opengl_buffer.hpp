#pragma once

#include "engine/renderer/buffer.hpp"
#include <glad/glad.h>

namespace hellix::renderer {

    class OpenGLVertexBuffer : public VertexBuffer {
    private:
        GLuint m_rendererID = 0;
        BufferLayout m_layout;

    public:
        // Construtor para buffer dinâmico
        explicit OpenGLVertexBuffer(uint32_t size);
        // Construtor para buffer estático com dados iniciais
        OpenGLVertexBuffer(const float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unbind() const override;

        void setData(const void* data, uint32_t size) override;

        [[nodiscard]] const BufferLayout& getLayout() const override { return m_layout; }
        void setLayout(const BufferLayout& layout) override { m_layout = layout; }
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    private:
        GLuint m_rendererID = 0;
        uint32_t m_count = 0;

    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        [[nodiscard]] uint32_t getCount() const override { return m_count; }
    };

}