#pragma once

#include "engine/core/application.hpp"
#include "engine/core/window.hpp"
#include "engine/core/buffer.hpp"
#include "engine/core/file_system.hpp"
#include "engine/core/timestep.hpp"
#include "engine/core/log.hpp"
#include "engine/core/layer_stack.hpp"

namespace hlx {
    using namespace hellix;
    using FileSystem = core::FileSystem;
    using Buffer = core::Buffer;
    using Application = core::Application;
    using Window = core::Window;
    using TimeStep = core::Timestep;
    using Log = core::Log;
    using LayerStack = core::LayerStack;
    using Layer = core::Layer;
}