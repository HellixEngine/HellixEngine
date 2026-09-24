#include "engine/core/input_manager.hpp"
#include <cmath>

namespace hellix::core::input {

    namespace {
        // Máscaras de bits locais para os botões do mouse
        constexpr uint32_t MOUSE_BUTTON_LEFT_BIT   = 1 << 0;
        constexpr uint32_t MOUSE_BUTTON_RIGHT_BIT  = 1 << 1;
        constexpr uint32_t MOUSE_BUTTON_MIDDLE_BIT = 1 << 2;

        // todo_: Substituir pela função de distância euclidiana da biblioteca de matemática da engine
        float calculateDistance(const Vec2i& a, const Vec2i& b) {
            float dx = static_cast<float>(a.first - b.first);
            float dy = static_cast<float>(a.second - b.second);
            return std::sqrt(dx * dx + dy * dy);
        }
    }

    InputManager::InputManager() {
        // Preenche o mapa de teclas do enum para os códigos do GLFW via X-Macro[cite: 4]
        #define X(name, code) m_keymap[Key::H_##name] = code;
        HLX_KEY_LIST
        #undef X

        m_currentState.reset(); 
        m_previousState.reset(); 
    }

    InputManager& InputManager::getInstance() {
        static InputManager instance; 
        return instance; 
    }

    // =========================================================
    // TECLADO: CONSULTAS CONTÍNUAS E TRANSIÇÕES
    // =========================================================

    bool InputManager::isKeyPressed(Key key) {
        return m_currentState[toIdx(key)]; 
    }

    bool InputManager::isKeyPressed(Key k1, Key k2) {
        return isKeyPressed(k1) || isKeyPressed(k2); 
    }

    bool InputManager::isKeyReleased(Key key) {
        return m_previousState[toIdx(key)] && !m_currentState[toIdx(key)]; 
    }

    bool InputManager::isKeyReleased(Key k1, Key k2) {
        return isKeyReleased(k1) || isKeyReleased(k2); 
    }

    bool InputManager::isJustKeyPressed(Key key) {
        return !m_previousState[toIdx(key)] && m_currentState[toIdx(key)]; 
    }

    bool InputManager::isJustKeyPressed(Key k1, Key k2) {
        return isJustKeyPressed(k1) || isJustKeyPressed(k2); 
    }

    bool InputManager::isJustKeyReleased(Key key) {
        return !m_currentState[toIdx(key)] && m_previousState[toIdx(key)]; 
    }

    bool InputManager::isJustKeyReleased(Key k1, Key k2) {
        return isJustKeyReleased(k1) || isJustKeyReleased(k2); 
    }

    // =========================================================
    // ATALHOS COM MODIFICADORES
    // =========================================================

    bool InputManager::isShortcutJustPressed(Key modifier, Key key) {
        return isKeyPressed(modifier) && isJustKeyPressed(key); 
    }

    bool InputManager::isShortcutReleased(Key modifier, Key key) {
        return isKeyPressed(modifier) && isKeyReleased(key); 
    }

    // =========================================================
    // MAPEAMENTO DE AÇÕES
    // =========================================================

    void InputManager::addBinding(const std::string& action, KeyBind bind) {
        m_bindings.insert_or_assign(action, bind); 
    }

    std::string InputManager::addBindingR(const std::string &action, KeyBind bind) {
        m_bindings.insert_or_assign(action, bind);
        return action;
    }

    bool InputManager::isActionPressed(const std::string& action) {
        auto it = m_bindings.find(action); 
        if (it == m_bindings.end()) return false; 

        const KeyBind& b = it->second; 
        if (b.modifier != Key::_COUNT) { 
            return isKeyPressed(b.modifier) && isKeyPressed(b.mainKey); 
        }
        return isKeyPressed(b.mainKey); 
    }

    bool InputManager::isActionJustPressed(const std::string& action) {
        auto it = m_bindings.find(action); 
        if (it == m_bindings.end()) return false; 

        const KeyBind& b = it->second; 
        if (b.modifier != Key::_COUNT) { 
            return isShortcutJustPressed(b.modifier, b.mainKey); 
        }
        return isJustKeyPressed(b.mainKey); 
    }

    bool InputManager::isActionJustReleased(const std::string& action) {
        auto it = m_bindings.find(action); 
        if (it == m_bindings.end()) return false; 

        const KeyBind& b = it->second; 
        if (b.modifier != Key::_COUNT) { 
            return isShortcutReleased(b.modifier, b.mainKey); 
        }
        return isJustKeyReleased(b.mainKey); 
    }

    bool InputManager::isActionReleased(const std::string& action) {
        return isActionJustReleased(action); 
    }

    // =========================================================
    // EIXOS 1D E 2D
    // =========================================================

    float InputManager::getInputValueF(Key k1, Key k2) {
        float input = 0.0f; 
        if (isKeyPressed(k1)) input -= 1.0f; 
        if (isKeyPressed(k2)) input += 1.0f; 
        return input; 
    }

    float InputManager::getInputValueF(Key k1, Key k2, Key altK1, Key altK2) {
        float input = 0.0f; 
        if (isKeyPressed(k1) || isKeyPressed(altK1)) input -= 1.0f; 
        if (isKeyPressed(k2) || isKeyPressed(altK2)) input += 1.0f; 
        return input; 
    }

    int InputManager::getInputValue(Key k1, Key k2) {
        int input = 0; 
        if (isKeyPressed(k1)) input -= 1; 
        if (isKeyPressed(k2)) input += 1; 
        return input; 
    }

    int InputManager::getInputValue(Key k1, Key k2, Key altK1, Key altK2) {
        int input = 0; 
        if (isKeyPressed(k1) || isKeyPressed(altK1)) input -= 1; 
        if (isKeyPressed(k2) || isKeyPressed(altK2)) input += 1; 
        return input; 
    }

    Vec2i InputManager::getInputValue2D(Key left, Key right, Key up, Key down) {
        // Y positivo para cima (up = +1, down = -1)[cite: 4]
        return { getInputValue(left, right), getInputValue(down, up) };
    }

    Vec2i InputManager::getInputValue2D(Key left, Key right, Key up, Key down,
                                        Key altLeft, Key altRight, Key altUp, Key altDown) {
        return { getInputValue(left, right, altLeft, altRight), getInputValue(down, up, altDown, altUp) };
    }

    Vec2f InputManager::getInputValueF2D(Key left, Key right, Key up, Key down) {
        return { getInputValueF(left, right), getInputValueF(down, up) };
    }

    Vec2f InputManager::getInputValueF2D(Key left, Key right, Key up, Key down,
                                         Key altLeft, Key altRight, Key altUp, Key altDown) {
        return { getInputValueF(left, right, altLeft, altRight), getInputValueF(down, up, altDown, altUp) };
    }

    Vec2i InputManager::getInputAxes() {
        return getInputValue2D(Key::H_A, Key::H_D, Key::H_W, Key::H_S,
                               Key::H_LEFT, Key::H_RIGHT, Key::H_UP, Key::H_DOWN); 
    }

    Vec2f InputManager::getInputAxesF() {
        return getInputValueF2D(Key::H_A, Key::H_D, Key::H_W, Key::H_S,
                                Key::H_LEFT, Key::H_RIGHT, Key::H_UP, Key::H_DOWN); 
    }

    // =========================================================
    // MOUSE: CONSULTAS DE BOTÕES
    // =========================================================

    bool InputManager::isMouseLeftPressed() const {
        return (m_currentMouseState & MOUSE_BUTTON_LEFT_BIT) && !(m_lastMouseState & MOUSE_BUTTON_LEFT_BIT);
    }

    bool InputManager::isMouseLeftReleased() const {
        return !(m_currentMouseState & MOUSE_BUTTON_LEFT_BIT) && (m_lastMouseState & MOUSE_BUTTON_LEFT_BIT);
    }

    bool InputManager::isMousePressed(MouseButton button) const {
        return (m_currentMouseState & (1 << static_cast<int>(button))) && !(m_lastMouseState & (1 << static_cast<int>(button)));
    }

    bool InputManager::isMouseReleased(MouseButton button) const {
        return !(m_currentMouseState & (1 << static_cast<int>(button))) && (m_lastMouseState & (1 << static_cast<int>(button)));
    }

    bool InputManager::isMouseRightPressed() const {
        return (m_currentMouseState & MOUSE_BUTTON_RIGHT_BIT) && !(m_lastMouseState & MOUSE_BUTTON_RIGHT_BIT);
    }

    bool InputManager::isMouseRightReleased() const {
        return !(m_currentMouseState & MOUSE_BUTTON_RIGHT_BIT) && (m_lastMouseState & MOUSE_BUTTON_RIGHT_BIT);
    }

    bool InputManager::isMouseMiddlePressed() const {
        return (m_currentMouseState & MOUSE_BUTTON_MIDDLE_BIT) && !(m_lastMouseState & MOUSE_BUTTON_MIDDLE_BIT);
    }

    bool InputManager::isMouseMiddleReleased() const {
        return !(m_currentMouseState & MOUSE_BUTTON_MIDDLE_BIT) && (m_lastMouseState & MOUSE_BUTTON_MIDDLE_BIT);
    }

    bool InputManager::isMouseLeftDown() const {
        return (m_currentMouseState & MOUSE_BUTTON_LEFT_BIT) != 0;
    }

    bool InputManager::isMouseRightDown() const {
        return (m_currentMouseState & MOUSE_BUTTON_RIGHT_BIT) != 0;
    }

    bool InputManager::isMouseMiddleDown() const {
        return (m_currentMouseState & MOUSE_BUTTON_MIDDLE_BIT) != 0;
    }

    bool InputManager::isMouseButtonDown(MouseButton button) const {
        return (m_currentMouseState & (1 << static_cast<int>(button))) != 0;
    }

    // =========================================================
    // REFLEXÃO DE NOMES E ATUALIZAÇÃO VIA GLFW
    // =========================================================

    const char* InputManager::getKeyName(Key key) const {
        switch (key) {
            #define X(name, code) case Key::H_##name: return #name;
            HLX_KEY_LIST
            #undef X
            default: return "UNKNOWN_KEY";
        }
    }

    const char* InputManager::getMouseButtonName(MouseButton button) const {
        switch (button) {
            #define X(name, code) case MouseButton::H_##name: return #name;
            HLX_MOUSE_BUTTON_LIST
            #undef X
            default: return "UNKNOWN_MOUSE_BUTTON";
        }
    }

    void InputManager::update(GLFWwindow* windowHandle) {
        if (!windowHandle) return;

        // 1. Salva o snapshot anterior do teclado
        m_previousState = m_currentState;

        // 2. Consulta de teclado via GLFW Polling
        for (const auto& [hlxKey, glfwKeyCode] : m_keymap) {
            if (glfwKeyCode == GLFW_KEY_UNKNOWN) continue;

            int state = glfwGetKey(windowHandle, glfwKeyCode);
            m_currentState[toIdx(hlxKey)] = (state == GLFW_PRESS || state == GLFW_REPEAT);
        }

        // 3. Salva snapshot anterior do mouse e consulta estado atual via GLFW
        m_lastMouseState = m_currentMouseState;
        m_currentMouseState = 0;

        if (glfwGetMouseButton(windowHandle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            m_currentMouseState |= MOUSE_BUTTON_LEFT_BIT;
        }
        if (glfwGetMouseButton(windowHandle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            m_currentMouseState |= MOUSE_BUTTON_RIGHT_BIT;
        }
        if (glfwGetMouseButton(windowHandle, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            m_currentMouseState |= MOUSE_BUTTON_MIDDLE_BIT;
        }
        double cursorX = 0.0;
        double cursorY = 0.0;
        // 4. Consulta de posição do cursor via GLFW (substituindo SDL_GetMouseState)
        glfwGetCursorPos(windowHandle, &cursorX, &cursorY);

        m_mousePosition = { static_cast<int>(cursorX), static_cast<int>(cursorY) };

        // 5. Cálculo do Delta de movimento do mouse
        m_mouseDelta = {
            m_mousePosition.first - m_lastMousePos.first,
            m_mousePosition.second - m_lastMousePos.second
        };
        m_lastMousePos = m_mousePosition;

        // 6. Lógica de detecção de arrasto (Drag) com botão esquerdo
        if (isMouseLeftPressed()) {
            m_dragStartPosLeft = m_mousePosition;
        }

        if (isMouseLeftDown()) {
            if (!m_isDraggingLeft) {
                if (calculateDistance(m_mousePosition, m_dragStartPosLeft) > static_cast<float>(m_dragThreshold)) {
                    m_isDraggingLeft = true;
                }
            }
        } else {
            m_isDraggingLeft = false;
        }

        // 7. Cálculo do Delta de arraste (Drag Delta) se o arraste estiver em andamento
        if (m_isDraggingLeft) {
            m_dragDelta = {
                m_mousePosition.first - m_dragStartPosLeft.first,
                m_mousePosition.second - m_dragStartPosLeft.second
            };
        } else {
            m_dragDelta = { 0, 0 };
        }

    }

}