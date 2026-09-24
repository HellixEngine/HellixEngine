#pragma once

#include <cmath>
#include <iostream>

namespace hellix::math {

    /**
     * @brief Estrutura vetorial bidimensional genérica.
     * @tparam T Tipo numérico dos componentes (ex: float, int).
     */
    template<typename T>
    struct Vector2 {
        union {
            struct { T x, y; };
            T data[2];
        };

        constexpr Vector2() = default;
        constexpr Vector2(T inX, T inY) : x(inX), y(inY) {}
        constexpr explicit Vector2(T scalar) : x(scalar), y(scalar) {}

        // =========================================================
        // OPERADORES ARITMÉTICOS (VETOR vs VETOR)
        // =========================================================
        constexpr Vector2 operator+(const Vector2& other) const { return {x + other.x, y + other.y}; }
        constexpr Vector2 operator-(const Vector2& other) const { return {x - other.x, y - other.y}; }
        constexpr Vector2 operator*(const Vector2& other) const { return {x * other.x, y * other.y}; }
        constexpr Vector2 operator/(const Vector2& other) const { return {x / other.x, y / other.y}; }

        constexpr Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
        constexpr Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
        constexpr Vector2& operator*=(const Vector2& other) { x *= other.x; y *= other.y; return *this; }
        constexpr Vector2& operator/=(const Vector2& other) { x /= other.x; y /= other.y; return *this; }

        // =========================================================
        // OPERADORES ARITMÉTICOS (VETOR vs ESCALAR)
        // =========================================================
        constexpr Vector2 operator+(T scalar) const { return {x + scalar, y + scalar}; }
        constexpr Vector2 operator-(T scalar) const { return {x - scalar, y - scalar}; }
        constexpr Vector2 operator*(T scalar) const { return {x * scalar, y * scalar}; }
        constexpr Vector2 operator/(T scalar) const { return {x / scalar, y / scalar}; }

        constexpr Vector2& operator+=(T scalar) { x += scalar; y += scalar; return *this; }
        constexpr Vector2& operator-=(T scalar) { x -= scalar; y -= scalar; return *this; }
        constexpr Vector2& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
        constexpr Vector2& operator/=(T scalar) { x /= scalar; y /= scalar; return *this; }

        constexpr Vector2 operator-() const { return {-x, -y}; }

        // =========================================================
        // COMPARAÇÃO
        // =========================================================
        constexpr bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
        constexpr bool operator!=(const Vector2& other) const { return !(*this == other); }

        // =========================================================
        // OPERAÇÕES GEOMÉTRICAS
        // =========================================================
        [[nodiscard]] constexpr T lengthSquared() const {
            return (x * x) + (y * y);
        }

        [[nodiscard]] float length() const {
            return std::sqrt(static_cast<float>(lengthSquared()));
        }

        [[nodiscard]] Vector2 normalized() const {
            float len = length();
            if (len > 0.0f) {
                float invLen = 1.0f / len;
                return {static_cast<T>(x * invLen), static_cast<T>(y * invLen)};
            }
            return *this;
        }

        static constexpr T dot(const Vector2& a, const Vector2& b) {
            return (a.x * b.x) + (a.y * b.y);
        }

        static float distance(const Vector2& a, const Vector2& b) {
            return (a - b).length();
        }

        static constexpr T distanceSquared(const Vector2& a, const Vector2& b) {
            return (a - b).lengthSquared();
        }
    };

    // Aliases explícitos
    using Vec2  = Vector2<float>;
    using Vec2i = Vector2<int>;
    using Vec2u = Vector2<unsigned int>;

}

namespace hlx {
    using Vec2  = hellix::math::Vec2;
    using Vec2i = hellix::math::Vec2i;
    using Vec2u = hellix::math::Vec2u;
}