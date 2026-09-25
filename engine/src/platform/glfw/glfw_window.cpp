#include "engine/core/window.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/image.hpp"
#include "engine/core/log.hpp"
#include "engine/events/application_event.hpp"
#include "engine/events/key_event.hpp"
#include "engine/events/mouse_event.hpp"

namespace hellix::core {

    static bool s_glfwInitialized = false;

    static void glfwErrorCallback(int error, const char* description) {
        HELLIX_ERROR("GLFW Error ({0}): {1}", error, description);

    }

    void Window::WindowDeleter::operator()(GLFWwindow* window) const {
        if (window) {
            glfwDestroyWindow(window);
        }
    }

    void Window::setIcon(std::string_view virtualPath) {
        if (virtualPath.empty()) {
            // Se o caminho for vazio, restaura o ícone padrão do sistema operacional.
            glfwSetWindowIcon(m_window.get(), 0, nullptr);
            return;
        }

        // GLFW espera a imagem sem inversão vertical e com 4 canais (RGBA).
        ImageData iconData = ImageLoader::load(virtualPath, false, 4);

        if (!iconData.isValid()) {
            HELLIX_WARN("GlfwWindow: Não foi possível definir o ícone da janela a partir de: {0}", virtualPath);
            return;
        }

        GLFWimage glfwImage;
        glfwImage.width = iconData.width;
        glfwImage.height = iconData.height;
        glfwImage.pixels = iconData.pixels;

        // Aplica o ícone na janela (a API aceita um array; aqui passamos uma imagem).
        glfwSetWindowIcon(m_window.get(), 1, &glfwImage);
        HELLIX_INFO("GlfwWindow: Ícone definido com sucesso: {0} ({1}x{2})",
                         virtualPath, iconData.width, iconData.height);

        // iconData é desalocada automaticamente aqui por RAII.
    }

    Window::Window(const WindowProps& props)
        : m_data{props.title, props.width, props.height, props.iconPath} {
        
        if (!s_glfwInitialized) {
            int success = glfwInit();
            if (!success) {
                glfwSetErrorCallback(glfwErrorCallback);
                HELLIX_ERROR("Falha ao inicializar GLFW!");
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
            HELLIX_ERROR("Falha ao criar a janela GLFW!");
            return;
        }

        m_window.reset(rawWindow);
        glfwMakeContextCurrent(rawWindow);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            HELLIX_ERROR("Falha ao inicializar GLAD!");
            return;
        }

        // Aplica o ícone se o caminho foi fornecido.
        if (!m_data.iconPath.empty()) {
            setIcon(m_data.iconPath);
        }

        glViewport(0, 0, static_cast<int>(m_data.width), static_cast<int>(m_data.height));// Define a viewport inicial
        glfwSwapInterval(1); // VSync ativo

        // Associa a estrutura m_data ao ponteiro interno GLFW da janela.
        glfwSetWindowUserPointer(rawWindow, &m_data);

        // Registro dos callbacks GLFW -> Hellix.

        // 1. Redimensionamento da janela.
        glfwSetWindowSizeCallback(m_window.get(), [](GLFWwindow* window, int width, int height) {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data->width = width;
            data->height = height;

            glViewport(0, 0, width, height);//redefine a viewport quando a janela for redimensionada

            events::WindowResizeEvent event(width, height);
            if (data->eventCallback) data->eventCallback(event);
        });

        // 2. Fechamento da janela.
        glfwSetWindowCloseCallback(m_window.get(), [](GLFWwindow* window) {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            events::WindowCloseEvent event;
            if (data->eventCallback) data->eventCallback(event);
        });

        // 3. Teclado.
        glfwSetKeyCallback(m_window.get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    events::KeyPressedEvent event(key, 0);
                    if (data->eventCallback) data->eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    events::KeyReleasedEvent event(key);
                    if (data->eventCallback) data->eventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    events::KeyPressedEvent event(key, 1);
                    if (data->eventCallback) data->eventCallback(event);
                    break;
                }
            }
        });

        // 4. Cliques do mouse.
        glfwSetMouseButtonCallback(m_window.get(), [](GLFWwindow* window, int button, int action, int mods) {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    events::MouseButtonPressedEvent event(button);
                    if (data->eventCallback) data->eventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    events::MouseButtonReleasedEvent event(button);
                    if (data->eventCallback) data->eventCallback(event);
                    break;
                }
                default: break;
            }
        });

        // 5. Posição do cursor.
        glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xpos, double ypos) {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            events::MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            if (data->eventCallback) data->eventCallback(event);
        });

        // 6. Rolagem do mouse.
        glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double xoffset, double yoffset) {
            auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

            events::MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
            if (data->eventCallback) data->eventCallback(event);
        });
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