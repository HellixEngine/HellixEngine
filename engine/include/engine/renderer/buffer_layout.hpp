#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace hellix::renderer {

    /**
     * @brief Tipos de dados fundamentais aceitos por sombreadores e buffers de vértice.
     */
    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    /**
     * @brief Retorna o tamanho em bytes ocupado por um determinado tipo de dado do sombreador.
     */
    [[nodiscard]] constexpr uint32_t shaderDataTypeSize(ShaderDataType type) noexcept {
        switch (type) {
            case ShaderDataType::Float:    return 4;
            case ShaderDataType::Float2:   return 4 * 2;
            case ShaderDataType::Float3:   return 4 * 3;
            case ShaderDataType::Float4:   return 4 * 4;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
            case ShaderDataType::Int:      return 4;
            case ShaderDataType::Int2:     return 4 * 2;
            case ShaderDataType::Int3:     return 4 * 3;
            case ShaderDataType::Int4:     return 4 * 4;
            case ShaderDataType::Bool:     return 1;
            default:                       return 0;
        }
    }

    /**
     * @brief Descreve um atributo individual contido na estrutura de um vértice.
     */
    struct BufferElement {
        std::string name;
        ShaderDataType type;
        uint32_t size;
        size_t offset = 0;
        bool normalized = false;

        BufferElement() = default;

        BufferElement(ShaderDataType inType, std::string inName, bool inNormalized = false)
            : name(std::move(inName)), type(inType), size(shaderDataTypeSize(inType)), normalized(inNormalized) {}

        /**
         * @brief Retorna a quantidade de componentes escalares do tipo (ex: Float3 -> 3).
         */
        [[nodiscard]] constexpr uint32_t getComponentCount() const noexcept {
            switch (type) {
                case ShaderDataType::Float:   return 1;
                case ShaderDataType::Float2:  return 2;
                case ShaderDataType::Float3:  return 3;
                case ShaderDataType::Float4:  return 4;
                case ShaderDataType::Mat3:    return 3 * 3;
                case ShaderDataType::Mat4:    return 4 * 4;
                case ShaderDataType::Int:     return 1;
                case ShaderDataType::Int2:    return 2;
                case ShaderDataType::Int3:    return 3;
                case ShaderDataType::Int4:    return 4;
                case ShaderDataType::Bool:    return 1;
                default:                      return 0;
            }
        }
    };

    /**
     * @brief Descreve o arranjo contíguo e alinhamento dos atributos de cada vértice em um buffer.
     */
    class BufferLayout {
    private:
        std::vector<BufferElement> m_elements;
        uint32_t m_stride = 0;

        void calculateOffsetsAndStride() noexcept {
            size_t offset = 0;
            m_stride = 0;
            for (auto& element : m_elements) {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(elements) {
            calculateOffsetsAndStride();
        }

        [[nodiscard]] uint32_t getStride() const noexcept { return m_stride; }
        [[nodiscard]] const std::vector<BufferElement>& getElements() const noexcept { return m_elements; }

        [[nodiscard]] auto begin() noexcept { return m_elements.begin(); }
        [[nodiscard]] auto end() noexcept { return m_elements.end(); }
        [[nodiscard]] auto begin() const noexcept { return m_elements.begin(); }
        [[nodiscard]] auto end() const noexcept { return m_elements.end(); }
    };

}