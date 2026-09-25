#pragma once

#include <algorithm>
#include "engine/core/timestep.hpp"

namespace hellix::core {

    /**
     * @brief Temporizador de contagem progressiva para eventos de gameplay e lógica do Core.
     */
    class Timer {
    private:
        float m_targetTime = 0.0f;   // Duração alvo em segundos.
        float m_currentTime = 0.0f;  // Tempo acumulado.
        bool  m_active = false;      // Estado de execução.
        bool  m_loop = false;        // Reinicia ao atingir o alvo.

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
         * @brief Atualiza o temporizador com o tempo transcorrido em segundos.
         * @param deltaTime Tempo transcorrido desde a última atualização.
         * @return `true` se o temporizador disparou ou foi concluído neste frame.
         */
        bool update(float deltaTime) noexcept {
            if (!m_active || m_targetTime <= 0.0f) {
                return false;
            }

            m_currentTime += deltaTime;

            if (m_currentTime >= m_targetTime) {
                if (m_loop) {
                    // Mantém o excesso para evitar desvio temporal acumulado.
                    m_currentTime -= m_targetTime;
                } else {
                    m_currentTime = m_targetTime;
                    m_active = false;
                }
                return true; // Temporizador disparou ou foi concluído.
            }

            return false; // Temporizador ainda está em progresso.
        }

        /**
         * @brief Atualiza o temporizador usando diretamente um @ref Timestep.
         */
        bool update(Timestep ts) noexcept {
            return update(ts.GetSeconds());
        }

        // =========================================================
        // CONFIGURAÇÕES E GETTERS
        // =========================================================

        /** @brief Define a duração alvo do temporizador em segundos. */
        void setTarget(float seconds) noexcept {
            m_targetTime = std::max(0.0f, seconds);
        }

        /** @brief Define se o temporizador deve reiniciar automaticamente. */
        void setLoop(bool loop) noexcept {
            m_loop = loop;
        }

        /** @brief Informa se o temporizador está ativo. */
        [[nodiscard]] bool isActive() const noexcept { return m_active; }
        /** @brief Informa se o temporizador está configurado para repetir. */
        [[nodiscard]] bool isLoop() const noexcept { return m_loop; }
        /** @brief Informa se o temporizador terminou sem estar ativo. */
        [[nodiscard]] bool isFinished() const noexcept { return !m_active && m_currentTime >= m_targetTime && m_targetTime > 0.0f; }

        /** @brief Retorna a duração alvo em segundos. */
        [[nodiscard]] float getTargetTime() const noexcept { return m_targetTime; }
        /** @brief Retorna o tempo acumulado em segundos. */
        [[nodiscard]] float getCurrentTime() const noexcept { return m_currentTime; }
        /** @brief Retorna o tempo restante em segundos. */
        [[nodiscard]] float getTimeLeft() const noexcept { return std::max(0.0f, m_targetTime - m_currentTime); }

        /**
         * @brief Retorna o percentual de conclusão normalizado no intervalo [0.0f, 1.0f].
         */
        [[nodiscard]] float getProgress() const noexcept {
            if (m_targetTime <= 0.0f) return 1.0f;
            return std::clamp(m_currentTime / m_targetTime, 0.0f, 1.0f);
        }
    };

}
