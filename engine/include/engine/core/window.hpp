#pragma once

#include <functional>
#include <memory>
#include <string>

struct GLFWwindow;

namespace hellix::events{
    class Event;
}

namespace hellix::core {

    /**
     * @brief Propriedades usadas na criação de uma janela.
     */
    struct WindowProps {
        /** @brief Título da janela. */
        std::string title;
        /** @brief Largura da janela em pixels. */
        uint32_t width;
        /** @brief Altura da janela em pixels. */
        uint32_t height;
        /** @brief Caminho do ícone da janela. */
        std::string iconPath = "";

        WindowProps(std::string_view t = "Hellix Engine", uint32_t w = 1280, uint32_t h = 720, std::string_view InIconPath = "")
            : title(t), width(w), height(h), iconPath(InIconPath) {}
    };

    class Window {
    public:
        /**
         * @brief Tipo da função de callback dos eventos da janela.
         * @details Recebe uma referência para um objeto de evento e não retorna valor.
         */
        using EventCallbackFn = std::function<void(events::Event&)>;


        /**
         * @brief Cria uma janela com as propriedades informadas.
         * @param props Propriedades de criação da janela.
         */
        explicit Window(const WindowProps& props);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;


        /**
         * @brief Define ou altera o ícone da janela em tempo de execução.
         * @param virtualPath Caminho VFS (ex: "assets://icon.png"). Se vazio, restaura o padrão.
         */
        void setIcon(std::string_view virtualPath);

        /** @brief Processa eventos do sistema e apresenta o frame atual. */
        void onUpdate();
        /** @brief Informa se a janela recebeu uma solicitação de fechamento. */
        [[nodiscard]] bool shouldClose() const;
        /**
         * @brief Define a função de callback dos eventos da janela.
         * @param callback Função compatível com o tipo @ref EventCallbackFn.
         */
        void setEventCallback(const EventCallbackFn& callback){
            m_data.eventCallback = callback;
        }
        /** @brief Retorna a largura da janela em pixels. */
        [[nodiscard]] uint32_t getWidth() const { return m_data.width; }
        /** @brief Retorna a altura da janela em pixels. */
        [[nodiscard]] uint32_t getHeight() const { return m_data.height; }
        /** @brief Retorna o título da janela. */
        [[nodiscard]] const std::string& getTitle() const { return m_data.title; }
        /**
         * @brief Retorna o ponteiro nativo da janela GLFW.
         * @return Ponteiro para a instância GLFW associada à janela.
         */
        [[nodiscard]] GLFWwindow* getNativeWindow() const;

    private:
        struct WindowDeleter {
            void operator()(GLFWwindow* window) const;
        };
        /** @brief Estrutura interna que armazena as propriedades da janela. */
        struct WindowData {
            /** @brief Título da janela. */
            std::string title;
            /** @brief Largura da janela em pixels. */
            uint32_t width;
            /** @brief Altura da janela em pixels. */
            uint32_t height;
            /** @brief Caminho do ícone da janela. */
            std::string iconPath;
            /** @brief Função de callback usada para tratar eventos. */
            EventCallbackFn eventCallback;

        };

        WindowData m_data;
        std::unique_ptr<GLFWwindow, WindowDeleter> m_window;

    };

}