#pragma once

#include <cstdint>

#include "engine/events/event.hpp"
#include <sstream>

namespace hellix::events {

    /** @brief Evento emitido quando a janela muda de tamanho. */
    class WindowResizeEvent : public Event {
    public:
        /**
         * @brief Cria um evento de redimensionamento.
         * @param width Nova largura em pixels.
         * @param height Nova altura em pixels.
         */
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_width(width), m_height(height) {}

        /** @brief Retorna a nova largura da janela. */
        [[nodiscard]] uint32_t getWidth() const { return m_width; }
        /** @brief Retorna a nova altura da janela. */
        [[nodiscard]] uint32_t getHeight() const { return m_height; }

        /** @brief Retorna uma representação textual do redimensionamento. */
        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        uint32_t m_width, m_height;
    };

    /** @brief Evento emitido quando a janela deve ser fechada. */
    class WindowCloseEvent : public Event {
    public:
        /** @brief Cria um evento de fechamento da janela. */
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

}