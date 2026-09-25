#pragma once

#include <vector>
#include "engine/core/layer.hpp"

namespace hellix::core {

    /**
     * @brief Mantém a ordem de atualização das camadas e sobreposições.
     */
    class LayerStack {
    public:
        /** @brief Cria uma pilha de camadas vazia. */
        LayerStack();
        ~LayerStack();

        /** @brief Adiciona uma camada antes das sobreposições. */
        void pushLayer(Layer* layer);
        /** @brief Adiciona uma sobreposição ao final da pilha. */
        void pushOverlay(Layer* overlay);
        /** @brief Remove uma camada da pilha. */
        void popLayer(Layer* layer);
        /** @brief Remove uma sobreposição da pilha. */
        void popOverlay(Layer* overlay);

        auto begin() { return m_layers.begin(); }
        auto end() { return m_layers.end(); }
        auto rbegin() { return m_layers.rbegin(); }
        auto rend() { return m_layers.rend(); }

        auto begin() const { return m_layers.begin(); }
        auto end() const { return m_layers.end(); }
        auto rbegin() const { return m_layers.rbegin(); }
        auto rend() const { return m_layers.rend(); }

    private:
        std::vector<Layer*> m_layers;
        unsigned int m_layerInsertIndex = 0;
    };

}