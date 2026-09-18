#pragma once

#include <memory>

#include "timestep.hpp"
#include "engine/core/window.hpp"
#include "engine/events/event.hpp"
#include "engine/events/application_event.hpp"
namespace hellix::core {

    class Application {
    public:
        explicit Application(const std::string& name = "Hellix App");
        virtual ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void run();
        void close();

        virtual void onEvent(events::Event& e);

        virtual void onUpdate(Timestep ts) {}
        virtual void onRender() {}

        [[nodiscard]] Window& getWindow() { return *m_window; }
        [[nodiscard]] static Application& get() { return *s_instance; }

    private:
        bool onWindowClose(events::WindowCloseEvent& e);
        bool onWindowResize(events::WindowResizeEvent& e);
        static Application* s_instance;
        bool m_running = true;
        std::unique_ptr<Window> m_window;
        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ibo = 0;
        uint32_t m_shaderProgram = 0;

        void initQuadPipeline();
        void cleanupQuadPipeline();

        float m_lastFrameTime = 0.0f;

    };

}