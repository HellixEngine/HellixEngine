#pragma once

#include <cmath>
#include "engine/math/vec2.hpp"
#include "engine/math/vec3.hpp"

namespace hellix::math {

    /**
     * @brief Estrutura vetorial quadridimensional genérica.
     * @tparam T Tipo numérico dos componentes (ex: float, int).
     */
    template<typename T>
    struct Vector4 {
        union {
            struct { T x, y, z, w; };
            T data[4];
        };

        constexpr Vector4() = default;
        constexpr Vector4(T inX, T inY, T inZ, T inW) : x(inX), y(inY), z(inZ), w(inW) {}
        constexpr explicit Vector4(T scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

        /**
         * @brief Construtor de expansão a partir de um Vector3 e um valor W.
         */
        constexpr Vector4(const Vector3<T>& xyz, T inW) : x(xyz.x), y(xyz.y), z(xyz.z), w(inW) {}

        /**
         * @brief Construtor de expansão a partir de dois Vector2 (XY e ZW).
         */
        constexpr Vector4(const Vector2<T>& xy, const Vector2<T>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}

        /**
         * @brief Construtor de expansão a partir de um Vector2 e valores individuais para Z e W.
         */
        constexpr Vector4(const Vector2<T>& xy, T inZ, T inW) : x(xy.x), y(xy.y), z(inZ), w(inW) {}

        /** @name Operadores aritméticos entre vetores */
        ///@{
        constexpr Vector4 operator+(const Vector4& other) const { return {x + other.x, y + other.y, z + other.z, w + other.w}; }
        constexpr Vector4 operator-(const Vector4& other) const { return {x - other.x, y - other.y, z - other.z, w - other.w}; }
        constexpr Vector4 operator*(const Vector4& other) const { return {x * other.x, y * other.y, z * other.z, w * other.w}; }
        constexpr Vector4 operator/(const Vector4& other) const { return {x / other.x, y / other.y, z / other.z, w / other.w}; }

        constexpr Vector4& operator+=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        constexpr Vector4& operator-=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        constexpr Vector4& operator*=(const Vector4& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
        constexpr Vector4& operator/=(const Vector4& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
        ///@}

        /** @name Operadores aritméticos entre vetor e escalar */
        ///@{
        constexpr Vector4 operator+(T scalar) const { return {x + scalar, y + scalar, z + scalar, w + scalar}; }
        constexpr Vector4 operator-(T scalar) const { return {x - scalar, y - scalar, z - scalar, w - scalar}; }
        constexpr Vector4 operator*(T scalar) const { return {x * scalar, y * scalar, z * scalar, w * scalar}; }
        constexpr Vector4 operator/(T scalar) const { return {x / scalar, y / scalar, z / scalar, w / scalar}; }

        constexpr Vector4& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; w += scalar; return *this; }
        constexpr Vector4& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; w -= scalar; return *this; }
        constexpr Vector4& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this; }
        constexpr Vector4& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this; }

        constexpr Vector4 operator-() const { return {-x, -y, -z, -w}; }
        ///@}

        /** @name Comparação */
        ///@{
        constexpr bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        constexpr bool operator!=(const Vector4& other) const { return !(*this == other); }
        ///@}

        /** @name Operações geométricas */
        ///@{
        [[nodiscard]] constexpr T lengthSquared() const {
            return (x * x) + (y * y) + (z * z) + (w * w);
        }

        [[nodiscard]] float length() const {
            return std::sqrt(static_cast<float>(lengthSquared()));
        }

        [[nodiscard]] Vector4 normalized() const {
            float len = length();
            if (len > 0.0f) {
                float invLen = 1.0f / len;
                return {
                    static_cast<T>(x * invLen),
                    static_cast<T>(y * invLen),
                    static_cast<T>(z * invLen),
                    static_cast<T>(w * invLen)
                };
            }
            return *this;
        }

        /**
         * @brief Calcula o produto escalar quadridimensional.
         */
        static constexpr T dot(const Vector4& a, const Vector4& b) {
            return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
        }

        /**
         * @brief Calcula a distância euclidiana entre dois pontos 4D.
         */
        static float distance(const Vector4& a, const Vector4& b) {
            return (a - b).length();
        }

        /**
         * @brief Calcula o quadrado da distância euclidiana entre dois pontos 4D.
         */
        static constexpr T distanceSquared(const Vector4& a, const Vector4& b) {
            return (a - b).lengthSquared();
        }

        /** @name Subobjetos (swizzle básico) */
        ///@{
        [[nodiscard]] constexpr Vector2<T> xy() const { return {x, y}; }
        [[nodiscard]] constexpr Vector3<T> xyz() const { return {x, y, z}; }
        [[nodiscard]] constexpr Vector3<T> rgb() const { return {x, y, z}; }
        ///@}
    };

    /** @brief Alias de vetor 4D com componentes de ponto flutuante. */
    using Vec4  = Vector4<float>;
    /** @brief Alias de vetor 4D com componentes inteiros. */
    using Vec4i = Vector4<int>;
    /** @brief Alias de vetor 4D com componentes inteiros sem sinal. */
    using Vec4u = Vector4<unsigned int>;

}

namespace hlx {
    using Vec4  = hellix::math::Vec4;
    using Vec4i = hellix::math::Vec4i;
    using Vec4u = hellix::math::Vec4u;
}