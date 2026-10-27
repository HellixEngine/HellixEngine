#include "engine/renderer/buffer.hpp"
#include "../platform/opengl/opengl_buffer.hpp"
#include "engine/core/log.hpp"

namespace hellix::renderer {

    std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size) {
        // Futuramente integrado com RendererAPI::getAPI()
        return std::make_shared<OpenGLVertexBuffer>(size);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::create(const float* vertices, uint32_t size) {
        return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* indices, uint32_t count) {
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }

}