#include "engine/core/window.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace hellix::core {

    static bool s_glfwInitialized = false;

    void Window::WindowDeleter::operator()(GLFWwindow* window) const {
        if (window) {
            glfwDestroyWindow(window);
        }
    }

    Window::Window(const WindowProps& props)
        : m_data{props.title, props.width, props.height} {
        
        if (!s_glfwInitialized) {
            int success = glfwInit();
            if (!success) {
                std::cerr << "[HellixCore] Falha ao inicializar GLFW!\n";
                return;
            }
            s_glfwInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* rawWindow = glfwCreateWindow(
            static_cast<int>(m_data.width),
            static_cast<int>(m_data.height),
            m_data.title.c_str(),
            nullptr,
            nullptr
        );

        if (!rawWindow) {
            std::cerr << "[HellixCore] Falha ao criar a janela GLFW!\n";
            return;
        }

        m_window.reset(rawWindow);
        glfwMakeContextCurrent(rawWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "[HellixCore] Falha ao inicializar GLAD (OpenGL)!\n";
            return;
        }

        glViewport(0, 0, static_cast<int>(m_data.width), static_cast<int>(m_data.height));
        glfwSwapInterval(1); // VSync ativo
    }

    Window::~Window() {
        m_window.reset();
    }

    void Window::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_window.get());
    }

    bool Window::shouldClose() const {
        return m_window ? glfwWindowShouldClose(m_window.get()) : true;
    }

    GLFWwindow* Window::getNativeWindow() const {
        return m_window.get();
    }

}