#pragma once
#include "engine/core/buffer.hpp"
#include "engine/core/file_system.hpp"
#include "engine/core/timestep.hpp"
#include "engine/core/input_manager.hpp"
#include "engine/core/application.hpp"
#include "engine/core/window.hpp"
#include "engine/core/log.hpp"


/**
 * @brief The main namespace for the Hellix Engine to use in game development.
 */
namespace hlx {
    using namespace hellix;

    //using enum events::Key; // permite o acesso direto aos códigos de tecla sem precisar do prefixo dentro do namespace hlx
    using Key = core::input::Key;
    //using enum core::input::MouseButton; // permite o acesso direto aos códigos de tecla sem precisar do prefixo dentro do namespace hlx
    using MouseCode = core::input::MouseButton;
    using Input = core::input::InputManager;
    using TimeStep = core::Timestep;
    using Logger = core::Log;

    using FileSystem = core::FileSystem;
    using Buffer = core::Buffer;
}

