#include "engine/core/layer.hpp"

namespace hellix::core {

    Layer::Layer(std::string name)
        : m_debugName(std::move(name)) {}

}