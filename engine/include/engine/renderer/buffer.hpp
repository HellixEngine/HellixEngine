#pragma once

#include <cstdint>
#include <memory>
#include "engine/renderer/buffer_layout.hpp"

namespace hellix::renderer {

    /**
     * @brief Interface pura de gestão de memória de vértices na VRAM (VBO).
     */
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        /**
         * @brief Atualiza uma fatia ou a totalidade dos dados no buffer (modo dinâmico).
         */
        virtual void setData(const void* data, uint32_t size) = 0;

        [[nodiscard]] virtual const BufferLayout& getLayout() const = 0;
        virtual void setLayout(const BufferLayout& layout) = 0;

        // Métodos de Fábrica (Factory Pattern)
        static std::shared_ptr<VertexBuffer> create(uint32_t size);
        static std::shared_ptr<VertexBuffer> create(const float* vertices, uint32_t size);
    };

    /**
     * @brief Interface pura de gestão de buffer de índices (EBO / IBO).
     */
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        [[nodiscard]] virtual uint32_t getCount() const = 0;

        // Método de Fábrica (Factory Pattern)
        static std::shared_ptr<IndexBuffer> create(const uint32_t* indices, uint32_t count);
    };

}