#pragma once

#include <algorithm>
#include <cmath>
#include <numbers>

// Vetores fundamentais
#include "engine/math/vec2.hpp"
#include "engine/math/vec3.hpp"
#include "engine/math/vec4.hpp"
#include "engine/math/mat4.hpp"

namespace hellix::math {

    inline constexpr float PI         = std::numbers::pi_v<float>;
    inline constexpr float TWO_PI     = PI * 2.0f;
    inline constexpr float HALF_PI    = PI * 0.5f;
    inline constexpr float DEG_TO_RAD = PI / 180.0f;
    inline constexpr float RAD_TO_DEG = 180.0f / PI;

    /**
     * @brief Converte um valor angular de graus para radianos.
     */
    [[nodiscard]] constexpr float toRadians(float degrees) noexcept {
        return degrees * DEG_TO_RAD;
    }

    /**
     * @brief Converte um valor angular de radianos para graus.
     */
    [[nodiscard]] constexpr float toDegrees(float radians) noexcept {
        return radians * RAD_TO_DEG;
    }

    /**
     * @brief Restringe um valor entre um limite inferior e superior.
     */
    template<typename T>
    [[nodiscard]] constexpr T clamp(T value, T minVal, T maxVal) noexcept {
        return std::clamp(value, minVal, maxVal);
    }

    /**
     * @brief Interpolação linear escalar (lerp).
     * @param a Ponto inicial.
     * @param b Ponto final.
     * @param t Fator de interpolação [0.0, 1.0].
     */
    template<typename T>
    [[nodiscard]] constexpr T lerp(T a, T b, float t) noexcept {
        return static_cast<T>(a + (b - a) * t);
    }

    /**
     * @brief Interpolação linear para vetores 2D.
     */
    template<typename T>
    [[nodiscard]] constexpr Vector2<T> lerp(const Vector2<T>& a, const Vector2<T>& b, float t) noexcept {
        return { lerp(a.x, b.x, t), lerp(a.y, b.y, t) };
    }

    /**
     * @brief Interpolação linear para vetores 3D.
     */
    template<typename T>
    [[nodiscard]] constexpr Vector3<T> lerp(const Vector3<T>& a, const Vector3<T>& b, float t) noexcept {
        return { lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t) };
    }

    /**
     * @brief Interpolação linear para vetores 4D.
     */
    template<typename T>
    [[nodiscard]] constexpr Vector4<T> lerp(const Vector4<T>& a, const Vector4<T>& b, float t) noexcept {
        return { lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t), lerp(a.w, b.w, t) };
    }

    /**
     * @brief Verifica se dois valores de ponto flutuante são aproximadamente iguais considerando um epsilon.
     */
    [[nodiscard]] inline bool approximately(float a, float b, float epsilon = 1e-6f) noexcept {
        return std::abs(a - b) <= epsilon;
    }

}
