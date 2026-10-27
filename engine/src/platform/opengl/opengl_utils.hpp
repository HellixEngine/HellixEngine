#pragma once

#include <glad/glad.h>
#include "engine/renderer/buffer_layout.hpp"
#include "engine/core/log.hpp"

namespace hellix::renderer {

    [[nodiscard]] inline GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) noexcept {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
            case ShaderDataType::Bool:
                return GL_BOOL;
            default:
                break;
        }
        HELLIX_CORE_ERROR("Tipo de ShaderDataType desconhecido!");
        return 0;
    }

}