#pragma once

#include <cstdint>

#include "engine/events/event.hpp"
#include <sstream>

namespace hellix::events {

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height)
            : m_width(width), m_height(height) {}

        [[nodiscard]] uint32_t getWidth() const { return m_width; }
        [[nodiscard]] uint32_t getHeight() const { return m_height; }

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

    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

}