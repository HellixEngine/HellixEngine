#pragma once

namespace hellix::core {
    /**
     * @brief Representa o tempo transcorrido entre dois frames.
     */
    class Timestep {
    public:
        /**
         * @brief Cria um timestep.
         * @param time Tempo transcorrido em segundos.
         */
        constexpr Timestep(float time = 0.0f) : m_Time(time) {}

        /** @brief Converte o timestep para segundos. */
        constexpr operator float() const { return m_Time; }

        /** @brief Retorna o tempo transcorrido em segundos. */
        [[nodiscard]] constexpr float GetSeconds() const { return m_Time; }
        /** @brief Retorna o tempo transcorrido em milissegundos. */
        [[nodiscard]] constexpr float GetMilliseconds() const { return m_Time * 1000.0f; }
    private:
        float m_Time;
    };
}
