#pragma once
#include "engine/core/input_manager.hpp"

namespace hlx::input {
    using namespace hellix::core::input;

    using Input = InputManager;
    
    using enum Key;
    
    /// @brief Os enumeradores de mouse são expostos individualmente para evitar conflito com `Key::_COUNT`.
    using MouseButton::H_BUTTON_1;
    using MouseButton::H_BUTTON_2;
    using MouseButton::H_BUTTON_3;
    using MouseButton::H_BUTTON_4;
    using MouseButton::H_BUTTON_5;
    using MouseButton::H_BUTTON_6;
    using MouseButton::H_BUTTON_7;
    using MouseButton::H_BUTTON_8;
    using MouseButton::H_BUTTON_LEFT;
    using MouseButton::H_BUTTON_RIGHT;
    using MouseButton::H_BUTTON_MIDDLE;
}