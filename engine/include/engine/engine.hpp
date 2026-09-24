#pragma once
#include "engine/core/application.hpp"
#include "engine/core/window.hpp"
#include "engine/core/buffer.hpp"
#include "engine/core/file_system.hpp"
#include "engine/core/timestep.hpp"
#include "engine/core/input_manager.hpp"
#include "engine/core/log.hpp"
#include "engine/math/math.hpp"


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

namespace hlx::math {
    using namespace hellix::math;
    using Vec2  = math::Vec2;
    using Vec2i = math::Vec2i;
    using Vec2u = math::Vec2u;

    using Vec3  = math::Vec3;
    using Vec3i = math::Vec3i;
    using Vec3u = math::Vec3u;

    using Vec4  = math::Vec4;
    using Vec4i = math::Vec4i;
    using Vec4u = math::Vec4u;

    using Mat4  = math::Mat4;
}



