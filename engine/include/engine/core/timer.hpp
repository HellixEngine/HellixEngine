#pragma once

#include <algorithm>
#include "engine/core/timestep.hpp"

namespace hellix::core {

    /**
     * @brief Temporizador de contagem progressiva para eventos de gameplay e lógica do Core.
     */
    class Timer {
    private:
        float m_targetTime = 0.0f;   // Duração pretendida em segundos
        float m_currentTime = 0.0f;  // Tempo acumulado decorrido
        bool  m_active = false;      // Estado de execução do timer
        bool  m_loop = false;        // Se reinicia ciclicamente ao atingir o alvo

    public:
        Timer() = default;

        explicit Timer(float seconds, bool loop = false, bool autoStart = false)
            : m_targetTime(std::max(0.0f, seconds)), m_currentTime(0.0f), m_active(autoStart), m_loop(loop) {}

        // =========================================================
        // CONTROLO DE ESTADO
        // =========================================================

        /** @brief Inicia o timer do zero. */
        void start() noexcept {
            m_currentTime = 0.0f;
            m_active = true;
        }

        /** @brief Para o timer e zera a contagem acumulada. */
        void stop() noexcept {
            m_active = false;
            m_currentTime = 0.0f;
        }

        /** @brief Pausa a contagem mantendo o progresso atual. */
        void pause() noexcept {
            m_active = false;
        }

        /** @brief Retoma a contagem a partir do ponto onde foi pausado. */
        void resume() noexcept {
            m_active = true;
        }

        /** @brief Reinicia a contagem mantendo o estado de atividade atual. */
        void reset() noexcept {
            m_currentTime = 0.0f;
        }

        // =========================================================
        // ATUALIZAÇÃO
        // =========================================================

        /**
         * @brief Atualiza o temporizador com um delta time em segundos.
         * @return true se o timer disparou/completou neste frame.
         */
        bool update(float deltaTime) noexcept {
            if (!m_active || m_targetTime <= 0.0f) {
                return false;
            }

            m_currentTime += deltaTime;

            if (m_currentTime >= m_targetTime) {
                if (m_loop) {
                    // Mantém o excesso (overshoot) para evitar desvio temporal cumulativo
                    m_currentTime -= m_targetTime;
                } else {
                    m_currentTime = m_targetTime;
                    m_active = false;
                }
                return true;
            }

            return false;
        }

        /**
         * @brief Sobrecarga para uso direto com o Timestep da HellixEngine.
         */
        bool update(Timestep ts) noexcept {
            return update(ts.GetSeconds());
        }

        // =========================================================
        // CONFIGURAÇÕES E GETTERS
        // =========================================================

        void setTarget(float seconds) noexcept {
            m_targetTime = std::max(0.0f, seconds);
        }

        void setLoop(bool loop) noexcept {
            m_loop = loop;
        }

        [[nodiscard]] bool isActive() const noexcept { return m_active; }
        [[nodiscard]] bool isLoop() const noexcept { return m_loop; }
        [[nodiscard]] bool isFinished() const noexcept { return !m_active && m_currentTime >= m_targetTime && m_targetTime > 0.0f; }

        [[nodiscard]] float getTargetTime() const noexcept { return m_targetTime; }
        [[nodiscard]] float getCurrentTime() const noexcept { return m_currentTime; }
        [[nodiscard]] float getTimeLeft() const noexcept { return std::max(0.0f, m_targetTime - m_currentTime); }

        /**
         * @brief Retorna a percentagem de conclusão normalizada no intervalo [0.0f, 1.0f].
         */
        [[nodiscard]] float getProgress() const noexcept {
            if (m_targetTime <= 0.0f) return 1.0f;
            return std::clamp(m_currentTime / m_targetTime, 0.0f, 1.0f);
        }
    };

}
