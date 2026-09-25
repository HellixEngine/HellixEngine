#pragma once

#include <array>
#include <cmath>
#include <cstring>
#include "engine/math/vec3.hpp"
#include "engine/math/vec4.hpp"
#include "engine/math/mat4.hpp"

namespace hellix::math {

    /**
     * @brief Estrutura de matriz 4x4 em formato Column-Major (compatível com OpenGL/GLSL).
     * @details Os elementos internos são acessados via data[coluna][linha] ou data[coluna * 4 + linha].
     */
    struct Mat4 {
        // Disposição Column-Major: 4 colunas contendo 4 elementos cada (16 floats contíguos)
        float elements[16] = {
            1.0f, 0.0f, 0.0f, 0.0f, // Coluna 0
            0.0f, 1.0f, 0.0f, 0.0f, // Coluna 1
            0.0f, 0.0f, 1.0f, 0.0f, // Coluna 2
            0.0f, 0.0f, 0.0f, 1.0f  // Coluna 3
        };

        /** @name Construtores */
        ///@{

        /** @brief Inicializa como matriz identidade por omissão. */
        constexpr Mat4() = default;

        /**
         * @brief Constrói uma matriz diagonal com o valor escalar fornecido.
         * @param diagonal Valor atribuído aos elementos da diagonal principal.
         */
        constexpr explicit Mat4(float diagonal) {
            elements[0]  = diagonal; elements[1]  = 0.0f;     elements[2]  = 0.0f;     elements[3]  = 0.0f;
            elements[4]  = 0.0f;     elements[5]  = diagonal; elements[6]  = 0.0f;     elements[7]  = 0.0f;
            elements[8]  = 0.0f;     elements[9]  = 0.0f;     elements[10] = diagonal; elements[11] = 0.0f;
            elements[12] = 0.0f;     elements[13] = 0.0f;     elements[14] = 0.0f;     elements[15] = diagonal;
        }
        ///@}

        /** @name Acesso a dados (interoperabilidade com OpenGL) */
        ///@{

        /**
         * @brief Retorna o ponteiro direto para a memória contígua da matriz.
         * @details Ideal para glUniformMatrix4fv(location, 1, GL_FALSE, mat.valuePtr()).
         */
        [[nodiscard]] const float* valuePtr() const noexcept { return elements; }
        [[nodiscard]] float* valuePtr() noexcept { return elements; }

        /** @brief Acesso por índice contíguo linear [0 a 15]. */
        float& operator[](size_t index) noexcept { return elements[index]; }
        const float& operator[](size_t index) const noexcept { return elements[index]; }

        /** @brief Acesso por coordenadas de coluna e linha: mat(col, row). */
        float& operator()(size_t col, size_t row) noexcept { return elements[col * 4 + row]; }
        const float& operator()(size_t col, size_t row) const noexcept { return elements[col * 4 + row]; }
        ///@}

        /** @name Multiplicação de matrizes e vetores */
        ///@{

        Mat4 operator*(const Mat4& other) const noexcept {
            Mat4 result(0.0f);
            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    float sum = 0.0f;
                    for (int k = 0; k < 4; ++k) {
                        sum += elements[k * 4 + row] * other.elements[col * 4 + k];
                    }
                    result.elements[col * 4 + row] = sum;
                }
            }
            return result;
        }

        Mat4& operator*=(const Mat4& other) noexcept {
            *this = *this * other;
            return *this;
        }

        Vector4<float> operator*(const Vector4<float>& v) const noexcept {
            return {
                elements[0] * v.x + elements[4] * v.y + elements[8]  * v.z + elements[12] * v.w,
                elements[1] * v.x + elements[5] * v.y + elements[9]  * v.z + elements[13] * v.w,
                elements[2] * v.x + elements[6] * v.y + elements[10] * v.z + elements[14] * v.w,
                elements[3] * v.x + elements[7] * v.y + elements[11] * v.z + elements[15] * v.w
            };
        }
        ///@}

        /** @name Operações de transformação espacial */
        ///@{

        /** @brief Retorna uma matriz identidade. */
        static constexpr Mat4 identity() noexcept {
            return Mat4(1.0f);
        }

        /**
         * @brief Aplica uma translação sobre a matriz fornecida.
         */
        static Mat4 translate(const Mat4& mat, const Vec3& translation) noexcept {
            Mat4 result = mat;
            result.elements[12] = mat.elements[0] * translation.x + mat.elements[4] * translation.y + mat.elements[8]  * translation.z + mat.elements[12];
            result.elements[13] = mat.elements[1] * translation.x + mat.elements[5] * translation.y + mat.elements[9]  * translation.z + mat.elements[13];
            result.elements[14] = mat.elements[2] * translation.x + mat.elements[6] * translation.y + mat.elements[10] * translation.z + mat.elements[14];
            result.elements[15] = mat.elements[3] * translation.x + mat.elements[7] * translation.y + mat.elements[11] * translation.z + mat.elements[15];
            return result;
        }

        /**
         * @brief Aplica uma escala sobre a matriz fornecida.
         */
        static Mat4 scale(const Mat4& mat, const Vec3& scale) noexcept {
            Mat4 result;
            result.elements[0]  = mat.elements[0]  * scale.x;
            result.elements[1]  = mat.elements[1]  * scale.x;
            result.elements[2]  = mat.elements[2]  * scale.x;
            result.elements[3]  = mat.elements[3]  * scale.x;

            result.elements[4]  = mat.elements[4]  * scale.y;
            result.elements[5]  = mat.elements[5]  * scale.y;
            result.elements[6]  = mat.elements[6]  * scale.y;
            result.elements[7]  = mat.elements[7]  * scale.y;

            result.elements[8]  = mat.elements[8]  * scale.z;
            result.elements[9]  = mat.elements[9]  * scale.z;
            result.elements[10] = mat.elements[10] * scale.z;
            result.elements[11] = mat.elements[11] * scale.z;

            result.elements[12] = mat.elements[12];
            result.elements[13] = mat.elements[13];
            result.elements[14] = mat.elements[14];
            result.elements[15] = mat.elements[15];
            return result;
        }

        /**
         * @brief Cria uma rotação em torno de um eixo unitário arbitrário (ângulo em radianos).
         */
        static Mat4 rotate(const Mat4& mat, float radians, const Vec3& axis) noexcept {
            float c = std::cos(radians);
            float s = std::sin(radians);
            Vec3 a = axis.normalized();

            Mat4 rot(0.0f);
            rot.elements[0] = a.x * a.x * (1.0f - c) + c;
            rot.elements[1] = a.x * a.y * (1.0f - c) + a.z * s;
            rot.elements[2] = a.x * a.z * (1.0f - c) - a.y * s;

            rot.elements[4] = a.y * a.x * (1.0f - c) - a.z * s;
            rot.elements[5] = a.y * a.y * (1.0f - c) + c;
            rot.elements[6] = a.y * a.z * (1.0f - c) + a.x * s;

            rot.elements[8]  = a.z * a.x * (1.0f - c) + a.y * s;
            rot.elements[9]  = a.z * a.y * (1.0f - c) - a.x * s;
            rot.elements[10] = a.z * a.z * (1.0f - c) + c;

            rot.elements[15] = 1.0f;

            return mat * rot;
        }
        ///@}

        /** @name Projeções (espaço de recorte OpenGL: [-1, 1]) */
        ///@{

        /**
         * @brief Gera uma matriz de projeção ortográfica (padrão 2D ou UI).
         */
        static Mat4 ortho(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f) noexcept {
            Mat4 result(1.0f);
            result.elements[0]  = 2.0f / (right - left);
            result.elements[5]  = 2.0f / (top - bottom);
            result.elements[10] = -2.0f / (zFar - zNear);

            result.elements[12] = -(right + left) / (right - left);
            result.elements[13] = -(top + bottom) / (top - bottom);
            result.elements[14] = -(zFar + zNear) / (zFar - zNear);
            return result;
        }

        /**
         * @brief Gera uma matriz de projeção em perspectiva (padrão 3D).
         * @param fovYRad Campo de visão vertical em radianos.
         * @param aspect Proporção de tela (largura / altura).
         * @param zNear Plano de corte próximo (near plane).
         * @param zFar Plano de corte distante (far plane).
         */
        static Mat4 perspective(float fovYRad, float aspect, float zNear, float zFar) noexcept {
            Mat4 result(0.0f);
            float tanHalfFov = std::tan(fovYRad * 0.5f);

            result.elements[0]  = 1.0f / (aspect * tanHalfFov);
            result.elements[5]  = 1.0f / (tanHalfFov);
            result.elements[10] = -(zFar + zNear) / (zFar - zNear);
            result.elements[11] = -1.0f;
            result.elements[14] = -(2.0f * zFar * zNear) / (zFar - zNear);
            return result;
        }
        ///@}
    };

}
