#pragma once

#include <memory>
#include "engine/core/window.hpp"

namespace hellix::core {

    class Application {
    public:
        explicit Application(const std::string& name = "Hellix App");
        virtual ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run();
        void close();

        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onEvent(events::Event& e) {}

        [[nodiscard]] Window& getWindow() { return *m_window; }
        [[nodiscard]] static Application& get() { return *s_instance; }

    private:
        static Application* s_instance;
        bool m_running = true;
        std::unique_ptr<Window> m_window;
        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ibo = 0;
        uint32_t m_shaderProgram = 0;

        void initQuadPipeline();
        void cleanupQuadPipeline();
    };

}