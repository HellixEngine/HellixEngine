#pragma once

#include "engine/events/event.hpp"
#include <sstream>

namespace hellix::events {
    /** @brief Classe base para eventos relacionados ao teclado. */
    class KeyEvent : public Event {
    public:
        /** @brief Retorna o código da tecla associado ao evento. */
        [[nodiscard]] int getKeyCode() const { return m_keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        /** @brief Cria um evento de teclado com o código informado. */
        explicit KeyEvent(int keycode) : m_keyCode(keycode) {}
        int m_keyCode;
    };

    /** @brief Evento emitido quando uma tecla é pressionada. */
    class KeyPressedEvent : public KeyEvent {
    public:
        /**
         * @brief Cria um evento de tecla pressionada.
         * @param keycode Código da tecla.
         * @param repeatCount Quantidade de repetições geradas pelo sistema.
         */
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent(keycode), m_repeatCount(repeatCount) {}

        [[nodiscard]] int getRepeatCount() const { return m_repeatCount; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_repeatCount;
    };

    /** @brief Evento emitido quando uma tecla é liberada. */
    class KeyReleasedEvent : public KeyEvent {
    public:
        /** @brief Cria um evento de tecla liberada. */
        explicit KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    /** @brief Evento emitido por entrada de texto do teclado. */
    class KeyTypedEvent : public KeyEvent {
    public:
        /** @brief Cria um evento de tecla digitada. */
        explicit KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };

}
        /** @brief Retorna a quantidade de repetições da tecla. */