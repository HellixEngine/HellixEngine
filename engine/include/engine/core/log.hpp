/**
 * @file log.hpp
 * @author Julio Daniel
 * @brief ${END}
 * @date 18/09/2026
 *
 * Copyright (c) 2026 Julio Daniel. All rights reserved. / Todos os direitos reservados.
 */
#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace hellix::core {
    class Log {
    public:
        static void init();

        [[nodiscard]] static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
        [[nodiscard]] static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}

#define HELLIX_TRACE(...)    ::hellix::core::Log::getCoreLogger()->trace(__VA_ARGS__)
#define HELLIX_INFO(...)     ::hellix::core::Log::getCoreLogger()->info(__VA_ARGS__)
#define HELLIX_WARN(...)     ::hellix::core::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HELLIX_ERROR(...)    ::hellix::core::Log::getCoreLogger()->error(__VA_ARGS__)
#define HELLIX_CRITICAL(...) ::hellix::core::Log::getCoreLogger()->critical(__VA_ARGS__)

// Macros para uso da Aplicação / Sandbox / Editor
#define HLX_TRACE(...)         ::hellix::core::Log::getClientLogger()->trace(__VA_ARGS__)
#define HLX_INFO(...)          ::hellix::core::Log::getClientLogger()->info(__VA_ARGS__)
#define HLX_WARN(...)          ::hellix::core::Log::getClientLogger()->warn(__VA_ARGS__)
#define HLX_ERROR(...)         ::hellix::core::Log::getClientLogger()->error(__VA_ARGS__)
#define HLX_CRITICAL(...)      ::hellix::core::Log::getClientLogger()->critical(__VA_ARGS__)
