#pragma once

#include <memory>

#include "engine/core/layer.hpp"
#include "engine/core/layer_stack.hpp"
#include "engine/core/timestep.hpp"
#include "engine/core/window.hpp"
#include "engine/events/event.hpp"
#include "engine/events/application_event.hpp"


namespace hellix::core {

    /**
     * @brief Ponto de entrada e ciclo principal de uma aplicação Hellix.
     */
    class Application {
    public:
        /**
         * @brief Cria uma aplicação.
         * @param name Nome usado na janela da aplicação.
         */
        explicit Application(const std::string& name = "Hellix App");
        virtual ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        /** @brief Executa o ciclo principal da aplicação. */
        void run();
        /** @brief Solicita o encerramento da aplicação. */
        void close();

        /** @brief Encaminha um evento para a aplicação e suas camadas. */
        virtual void onEvent(events::Event& e);

        /** @brief Atualiza a lógica da aplicação a cada frame. */
        virtual void onUpdate(Timestep ts) {}
        /** @brief Renderiza o conteúdo da aplicação a cada frame. */
        virtual void onRender() {}

        /** @brief Retorna a janela principal da aplicação. */
        [[nodiscard]] Window& getWindow() { return *m_window; }
        /** @brief Retorna a instância global da aplicação. */
        [[nodiscard]] static Application& get() { return *s_instance; }

        /** @brief Adiciona uma camada ao fluxo de atualização. */
        void pushLayer(Layer* layer);
        /** @brief Adiciona uma camada de sobreposição. */
        void pushOverlay(Layer* overlay);

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

        LayerStack m_layerStack;

    };

}