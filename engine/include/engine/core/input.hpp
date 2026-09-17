#pragma once

#include "key_codes.hpp"
#include <utility>

namespace hellix::core::input {

    class Input {
    public:
        [[nodiscard]] static bool isKeyPressed(KeyCode key);
        [[nodiscard]] static bool isMouseButtonPressed(MouseCode button);
        [[nodiscard]] static std::pair<float, float> getMousePosition();
        [[nodiscard]] static float getMouseX();
        [[nodiscard]] static float getMouseY();
    };

}
