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
     *@brief Structure to hold properties for creating a window.
     */
    struct WindowProps {
        /**
         *@brief The title of the window.
         */
        std::string title;
        /**
         *@brief The width of the window in pixels.
         */
        uint32_t width;
        /**
         *@brief The height of the window in pixels.
         */
        uint32_t height;
        /**
         *@brief The path to the icon for the window.
         */
        std::string iconPath = "";

        WindowProps(std::string_view t = "Hellix Engine", uint32_t w = 1280, uint32_t h = 720, std::string_view InIconPath = "")
            : title(t), width(w), height(h), iconPath(InIconPath) {}
    };

    class Window {
    public:
        /**
         *@brief Event callback function type. It takes a reference to an Event object and returns void.
         */
        using EventCallbackFn = std::function<void(events::Event&)>;


        explicit Window(const WindowProps& props);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;


        /**
         * @brief Define ou altera o ícone da janela em tempo de execução.
         * @param virtualPath Caminho VFS (ex: "assets://icon.png"). Se vazio, restaura o padrão.
         */
        void setIcon(std::string_view virtualPath);

        void onUpdate();
        [[nodiscard]] bool shouldClose() const;
        /**
         *@brief Sets the event callback function for the window.
         *@param callback The callback function to be set. It should match the EventCallbackFn type.
         */
        void setEventCallback(const EventCallbackFn& callback){
            m_data.eventCallback = callback;
        }
        /**
         *@brief Retrieves the width of the window.
         *@return The width of the window in pixels.
         */
        [[nodiscard]] uint32_t getWidth() const { return m_data.width; }
        /**
         *@brief Retrieves the height of the window.
         *@return The height of the window in pixels.
         */
        [[nodiscard]] uint32_t getHeight() const { return m_data.height; }
        /**
         *@brief Retrieves the title of the window.
         *@return The title of the window as a string.
         */
        [[nodiscard]] const std::string& getTitle() const { return m_data.title; }
        /**
         *@brief Retrieves the native GLFW window pointer.
         *@return A pointer to the native GLFWwindow associated with this window.
         */
        [[nodiscard]] GLFWwindow* getNativeWindow() const;

    private:
        struct WindowDeleter {
            void operator()(GLFWwindow* window) const;
        };
        /**
         *@brief Data structure for storing window properties.
         */
        struct WindowData {
            /**
             *@brief The title of the window.
             */
            std::string title;
            /**
             *@brief The width of the window in pixels.
             */
            uint32_t width;
            /**
             *@brief The height of the window in pixels.
             */
            uint32_t height;
            /**
             *@brief The path to the icon for the window.
             */
            std::string iconPath;  // Path to the window icon
            /**
             *@brief Callback function for handling events.
             */
            EventCallbackFn eventCallback;

        };

        WindowData m_data;
        std::unique_ptr<GLFWwindow, WindowDeleter> m_window;

    };

}