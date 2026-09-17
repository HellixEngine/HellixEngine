#include "engine/core/input.hpp"
#include "engine/core/application.hpp"
#include <GLFW/glfw3.h>

namespace hellix::core::input {

    bool Input::isKeyPressed(KeyCode key) {
        auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(MouseCode button) {
        auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        int state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePosition() {
        auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return { static_cast<float>(xpos), static_cast<float>(ypos) };
    }

    float Input::getMouseX() {
        return getMousePosition().first;
    }

    float Input::getMouseY() {
        return getMousePosition().second;
    }

}