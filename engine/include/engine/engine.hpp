#pragma once

#include "engine/core/application.hpp"
#include "engine/core/window.hpp"
#include "engine/events/key_event.hpp"
#include "engine/core/input.hpp"

/**
 * @brief The main namespace for the Hellix Engine to use in game development.
 */
namespace hlx {
    using namespace hellix;
    //alternatica a using Key = events::KeyCode;
    //using enum events::KeyCode; // permite o acesso direto aos códigos de tecla sem precisar do prefixo dentro do namespace hlx

    //namespace input = core::input;
    using Key = core::input::KeyCode;
    using Mouse = core::input::MouseCode;
    using Input = core::input::Input;
    using TimeStep = core::Timestep;
}

