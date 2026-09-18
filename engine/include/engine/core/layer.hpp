#pragma once

#include <string>
#include "engine/core/timestep.hpp"
#include "engine/events/event.hpp"

namespace hellix::core {

    class Layer {
    public:
        explicit Layer(std::string name = "Layer");
        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(Timestep ts) {}
        virtual void onEvent(events::Event& event) {}

        [[nodiscard]] const std::string& getName() const { return m_debugName; }

    protected:
        std::string m_debugName;
    };

}