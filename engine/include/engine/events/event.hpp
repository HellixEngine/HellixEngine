#pragma once

#include <string>
#include <functional>

namespace hellix::events {

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication    = 1 << 0,
        EventCategoryInput          = 1 << 1,
        EventCategoryKeyboard       = 1 << 2,
        EventCategoryMouse          = 1 << 3,
        EventCategoryMouseButton    = 1 << 4
    };

// Macros utilitárias para evitar repetição de código boilerplate em eventos derivados
#define EVENT_CLASS_TYPE(type) \
    static EventType getStaticType() { return EventType::type; } \
    [[nodiscard]] EventType getEventType() const override { return getStaticType(); } \
    [[nodiscard]] const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    [[nodiscard]] int getCategoryFlags() const override { return category; }

    class Event {
    public:
        virtual ~Event() = default;

        bool handled = false;

        [[nodiscard]] virtual EventType getEventType() const = 0;
        [[nodiscard]] virtual const char* getName() const = 0;
        [[nodiscard]] virtual int getCategoryFlags() const = 0;
        [[nodiscard]] virtual std::string toString() const { return getName(); }

        [[nodiscard]] bool isInCategory(EventCategory category) const {
            return getCategoryFlags() & category;
        }
    };

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& event) : m_event(event) {}

        // F precisa ser uma função/lambda com assinatura bool(T&)
        template<typename T, typename F>
        bool dispatch(const F& func) {
            if (m_event.getEventType() == T::getStaticType()) {
                m_event.handled |= func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

}