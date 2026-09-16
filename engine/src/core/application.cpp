#include "engine/core/application.hpp"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace hellix::core {

    Application* Application::s_instance = nullptr;

    static const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main() {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    static const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        void main() {
            FragColor = vec4(0.2f, 0.65f, 0.85f, 1.0f); // Retângulo Ciano Hellix
        }
    )";

    Application::Application(const std::string& name) {
        s_instance = this;
        m_window = std::make_unique<Window>(WindowProps(name, 1280, 720));
        initQuadPipeline();
    }

    Application::~Application() {
        cleanupQuadPipeline();
        glfwTerminate();
        s_instance = nullptr;
    }

    void Application::close() {
        m_running = false;
    }

    void Application::initQuadPipeline() {
        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, vertexShader);
        glAttachShader(m_shaderProgram, fragmentShader);
        glLinkProgram(m_shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        float vertices[] = {
             0.5f,  0.5f, 0.0f,  // topo direito
             0.5f, -0.5f, 0.0f,  // base direita
            -0.5f, -0.5f, 0.0f,  // base esquerda
            -0.5f,  0.5f, 0.0f   // topo esquerdo
        };

        uint32_t indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ibo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Application::cleanupQuadPipeline() {
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
        if (m_ibo) glDeleteBuffers(1, &m_ibo);
        if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
    }

    void Application::run() {
        while (m_running) {
            if (m_window->shouldClose()) {
                m_running = false;
                break;
            }

            glClearColor(0.12f, 0.12f, 0.14f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(m_shaderProgram);
            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            onUpdate();
            onRender();

            m_window->onUpdate();
        }
    }

}