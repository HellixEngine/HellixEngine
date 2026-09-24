#pragma once

#include <cmath>
#include "engine/math/vec2.hpp"

namespace hellix::math {

    /**
     * @brief Estrutura vetorial tridimensional genérica.
     * @tparam T Tipo numérico dos componentes (ex: float, int).
     */
    template<typename T>
    struct Vector3 {
        union {
            struct { T x, y, z; };
            T data[3];
        };

        constexpr Vector3() = default;
        constexpr Vector3(T inX, T inY, T inZ) : x(inX), y(inY), z(inZ) {}
        constexpr explicit Vector3(T scalar) : x(scalar), y(scalar), z(scalar) {}

        /**
         * @brief Construtor de expansão a partir de um Vector2 e um valor Z.
         */
        constexpr Vector3(const Vector2<T>& xy, T inZ) : x(xy.x), y(xy.y), z(inZ) {}

        // =========================================================
        // OPERADORES ARITMÉTICOS (VETOR vs VETOR)
        // =========================================================
        constexpr Vector3 operator+(const Vector3& other) const { return {x + other.x, y + other.y, z + other.z}; }
        constexpr Vector3 operator-(const Vector3& other) const { return {x - other.x, y - other.y, z - other.z}; }
        constexpr Vector3 operator*(const Vector3& other) const { return {x * other.x, y * other.y, z * other.z}; }
        constexpr Vector3 operator/(const Vector3& other) const { return {x / other.x, y / other.y, z / other.z}; }

        constexpr Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        constexpr Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        constexpr Vector3& operator*=(const Vector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
        constexpr Vector3& operator/=(const Vector3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

        // =========================================================
        // OPERADORES ARITMÉTICOS (VETOR vs ESCALAR)
        // =========================================================
        constexpr Vector3 operator+(T scalar) const { return {x + scalar, y + scalar, z + scalar}; }
        constexpr Vector3 operator-(T scalar) const { return {x - scalar, y - scalar, z - scalar}; }
        constexpr Vector3 operator*(T scalar) const { return {x * scalar, y * scalar, z * scalar}; }
        constexpr Vector3 operator/(T scalar) const { return {x / scalar, y / scalar, z / scalar}; }

        constexpr Vector3& operator+=(T scalar) { x += scalar; y += scalar; z += scalar; return *this; }
        constexpr Vector3& operator-=(T scalar) { x -= scalar; y -= scalar; z -= scalar; return *this; }
        constexpr Vector3& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
        constexpr Vector3& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

        constexpr Vector3 operator-() const { return {-x, -y, -z}; }

        // =========================================================
        // COMPARAÇÃO
        // =========================================================
        constexpr bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
        constexpr bool operator!=(const Vector3& other) const { return !(*this == other); }

        // =========================================================
        // OPERAÇÕES GEOMÉTRICAS
        // =========================================================
        [[nodiscard]] constexpr T lengthSquared() const {
            return (x * x) + (y * y) + (z * z);
        }

        [[nodiscard]] float length() const {
            return std::sqrt(static_cast<float>(lengthSquared()));
        }

        [[nodiscard]] Vector3 normalized() const {
            float len = length();
            if (len > 0.0f) {
                float invLen = 1.0f / len;
                return {
                    static_cast<T>(x * invLen),
                    static_cast<T>(y * invLen),
                    static_cast<T>(z * invLen)
                };
            }
            return *this;
        }

        /**
         * @brief Calcula o produto escalar (dot product) entre dois vetores.
         */
        static constexpr T dot(const Vector3& a, const Vector3& b) {
            return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
        }

        /**
         * @brief Calcula o produto vetorial (cross product) entre dois vetores 3D.
         */
        static constexpr Vector3 cross(const Vector3& a, const Vector3& b) {
            return {
                (a.y * b.z) - (a.z * b.y),
                (a.z * b.x) - (a.x * b.z),
                (a.x * b.y) - (a.y * b.x)
            };
        }

        /**
         * @brief Calcula a distância euclidiana entre dois pontos 3D.
         */
        static float distance(const Vector3& a, const Vector3& b) {
            return (a - b).length();
        }

        /**
         * @brief Calcula o quadrado da distância euclidiana entre dois pontos 3D.
         */
        static constexpr T distanceSquared(const Vector3& a, const Vector3& b) {
            return (a - b).lengthSquared();
        }

        /**
         * @brief Retorna os componentes X e Y encapsulados num Vector2.
         */
        [[nodiscard]] constexpr Vector2<T> xy() const {
            return {x, y};
        }
    };

    // Aliases explícitos
    using Vec3  = Vector3<float>;
    using Vec3i = Vector3<int>;
    using Vec3u = Vector3<unsigned int>;

}

namespace hlx {
    using Vec3  = hellix::math::Vec3;
    using Vec3i = hellix::math::Vec3i;
    using Vec3u = hellix::math::Vec3u;
}