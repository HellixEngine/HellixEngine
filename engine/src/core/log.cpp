#include "../../include/engine/core/log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>



namespace hellix::core {

    std::shared_ptr<spdlog::logger> Log::s_coreLogger;
    std::shared_ptr<spdlog::logger> Log::s_clientLogger;

    void Log::init() {
        // Formato: [Horário] [Nome do Logger] Mensagem com cores ANSI
        spdlog::set_pattern("%^[%T] [%n] %v%$");

        s_coreLogger = spdlog::stdout_color_mt("HELLIX");
        s_coreLogger->set_level(spdlog::level::trace);

        s_clientLogger = spdlog::stdout_color_mt("APP");
        s_clientLogger->set_level(spdlog::level::trace);
    }

}
