#pragma once

#include <string>
#include "engine/core/timestep.hpp"
#include "engine/events/event.hpp"

namespace hellix::core {

    /**
     * @brief Unidade modular de atualização e tratamento de eventos.
     */
    class Layer {
    public:
        /**
         * @brief Cria uma camada.
         * @param name Nome usado para depuração.
         */
        explicit Layer(std::string name = "Layer");
        virtual ~Layer() = default;

        /** @brief Chamado quando a camada é anexada à pilha. */
        virtual void onAttach() {}
        /** @brief Chamado quando a camada é removida da pilha. */
        virtual void onDetach() {}
        /** @brief Atualiza a camada.
         * @param ts Tempo transcorrido desde o frame anterior.
         */
        virtual void onUpdate(Timestep ts) {}
        /** @brief Encaminha um evento para a camada. */
        virtual void onEvent(events::Event& event) {}

        /** @brief Retorna o nome da camada. */
        [[nodiscard]] const std::string& getName() const { return m_debugName; }

    protected:
        std::string m_debugName;
    };

}