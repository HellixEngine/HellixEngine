#pragma once

#include <vector>
#include "engine/core/layer.hpp"

namespace hellix::core {

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popLayer(Layer* layer);
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