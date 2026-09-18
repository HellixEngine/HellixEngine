#pragma once

#include "engine/events/event.hpp"
#include <sstream>

namespace hellix::events {




    class KeyEvent : public Event {
    public:
        [[nodiscard]] int getKeyCode() const { return m_keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        explicit KeyEvent(int keycode) : m_keyCode(keycode) {}
        int m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
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

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };

}