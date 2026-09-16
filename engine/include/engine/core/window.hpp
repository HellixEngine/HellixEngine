#pragma once

#include <memory>
#include <string>

struct GLFWwindow;

namespace hellix::core {

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;

        WindowProps(std::string t = "Hellix Engine", uint32_t w = 1280, uint32_t h = 720)
            : title(std::move(t)), width(w), height(h) {}
    };

    class Window {
    public:
        explicit Window(const WindowProps& props);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        void onUpdate();
        [[nodiscard]] bool shouldClose() const;

        [[nodiscard]] uint32_t getWidth() const { return m_data.width; }
        [[nodiscard]] uint32_t getHeight() const { return m_data.height; }
        [[nodiscard]] GLFWwindow* getNativeWindow() const;

    private:
        struct WindowDeleter {
            void operator()(GLFWwindow* window) const;
        };

        struct WindowData {
            std::string title;
            uint32_t width;
            uint32_t height;
        };

        WindowData m_data;
        std::unique_ptr<GLFWwindow, WindowDeleter> m_window;
    };

}