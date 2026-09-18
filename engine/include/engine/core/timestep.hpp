#pragma once

namespace hellix::core {
    class Timestep {
    public:
        constexpr Timestep(float time = 0.0f) : m_Time(time) {}

        constexpr operator float() const { return m_Time; }

        [[nodiscard]] constexpr float GetSeconds() const { return m_Time; }
        [[nodiscard]] constexpr float GetMilliseconds() const { return m_Time * 1000.0f; }
    private:
        float m_Time;
    };
}
