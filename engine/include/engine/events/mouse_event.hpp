#pragma once

#include "engine/events/event.hpp"
#include <sstream>

namespace hellix::events {

    /** @brief Evento emitido quando o cursor do mouse se move. */
    class MouseMovedEvent : public Event {
    public:
        /** @brief Cria um evento de movimento do mouse. */
        MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

        /** @brief Retorna a posição horizontal do cursor. */
        [[nodiscard]] float getX() const { return m_mouseX; }
        /** @brief Retorna a posição vertical do cursor. */
        [[nodiscard]] float getY() const { return m_mouseY; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_mouseX, m_mouseY;
    };

    /** @brief Evento emitido quando a roda do mouse é rolada. */
    class MouseScrolledEvent : public Event {
    public:
        /** @brief Cria um evento de rolagem do mouse. */
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_xOffset(xOffset), m_yOffset(yOffset) {}

        /** @brief Retorna o deslocamento horizontal da rolagem. */
        [[nodiscard]] float getXOffset() const { return m_xOffset; }
        /** @brief Retorna o deslocamento vertical da rolagem. */
        [[nodiscard]] float getYOffset() const { return m_yOffset; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_xOffset, m_yOffset;
    };

    /** @brief Classe base para eventos de botões do mouse. */
    class MouseButtonEvent : public Event {
    public:
        /** @brief Retorna o código do botão do mouse. */
        [[nodiscard]] int getMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

    protected:
        /** @brief Cria um evento de botão com o código informado. */
        explicit MouseButtonEvent(int button) : m_button(button) {}
        int m_button;
    };

    /** @brief Evento emitido quando um botão do mouse é pressionado. */
    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        /** @brief Cria um evento de botão pressionado. */
        explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    /** @brief Evento emitido quando um botão do mouse é liberado. */
    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        /** @brief Cria um evento de botão liberado. */
        explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}