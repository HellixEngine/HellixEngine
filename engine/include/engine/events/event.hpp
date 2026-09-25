#pragma once

#include <string>
#include <functional>

namespace hellix::events {

    /** @brief Tipos de eventos reconhecidos pela engine. */
    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    /** @brief Categorias usadas para filtrar eventos. */
    enum EventCategory {
        None = 0,
        EventCategoryApplication    = 1 << 0,
        EventCategoryInput          = 1 << 1,
        EventCategoryKeyboard       = 1 << 2,
        EventCategoryMouse          = 1 << 3,
        EventCategoryMouseButton    = 1 << 4
    };

    /**
     * @brief Gera os métodos de identificação de um evento derivado.
     * @param type Tipo definido em @ref EventType.
     */
#define EVENT_CLASS_TYPE(type) \
    static EventType getStaticType() { return EventType::type; } \
    [[nodiscard]] EventType getEventType() const override { return getStaticType(); } \
    [[nodiscard]] const char* getName() const override { return #type; }

    /**
     * @brief Gera o método de identificação das categorias do evento.
     * @param category Combinação de valores de @ref EventCategory.
     */
#define EVENT_CLASS_CATEGORY(category) \
    [[nodiscard]] int getCategoryFlags() const override { return category; }

    /**
     * @brief Classe base para todos os eventos da engine.
     */
    class Event {
    public:
        virtual ~Event() = default;

        /** @brief Indica se o evento já foi tratado por algum consumidor. */
        bool handled = false;

        /** @brief Retorna o tipo concreto do evento. */
        [[nodiscard]] virtual EventType getEventType() const = 0;
        /** @brief Retorna o nome textual do evento. */
        [[nodiscard]] virtual const char* getName() const = 0;
        /** @brief Retorna as categorias às quais o evento pertence. */
        [[nodiscard]] virtual int getCategoryFlags() const = 0;
        /** @brief Retorna uma representação textual do evento. */
        [[nodiscard]] virtual std::string toString() const { return getName(); }

        /**
         * @brief Verifica se o evento pertence a uma categoria.
         * @param category Categoria consultada.
         * @return `true` quando a categoria está presente.
         */
        [[nodiscard]] bool isInCategory(EventCategory category) const {
            return getCategoryFlags() & category;
        }
    };

    /**
     * @brief Encaminha um evento para uma função compatível com seu tipo.
     */
    class EventDispatcher {
    public:
        /** @brief Cria um despachante para o evento informado. */
        explicit EventDispatcher(Event& event) : m_event(event) {}

        /**
         * @brief Tenta despachar o evento para o tipo solicitado.
         * @tparam T Tipo concreto do evento.
         * @tparam F Tipo da função de tratamento, com assinatura compatível com `bool(T&)`.
         * @param func Função chamada quando o tipo do evento coincidir.
         * @return `true` quando o evento foi despachado.
         */
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