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

        m_currentState.reset(); //[cite: 4]
        m_previousState.reset(); //[cite: 4]
    }

    InputManager& InputManager::getInstance() {
        static InputManager instance; //[cite: 4]
        return instance; //[cite: 4]
    }

    // =========================================================
    // TECLADO: CONSULTAS CONTÍNUAS E TRANSIÇÕES
    // =========================================================

    bool InputManager::isKeyPressed(Key key) {
        return m_currentState[toIdx(key)]; //[cite: 4]
    }

    bool InputManager::isKeyPressed(Key k1, Key k2) {
        return isKeyPressed(k1) || isKeyPressed(k2); //[cite: 4]
    }

    bool InputManager::isKeyReleased(Key key) {
        return m_previousState[toIdx(key)] && !m_currentState[toIdx(key)]; //[cite: 4]
    }

    bool InputManager::isKeyReleased(Key k1, Key k2) {
        return isKeyReleased(k1) || isKeyReleased(k2); //[cite: 4]
    }

    bool InputManager::isJustKeyPressed(Key key) {
        return !m_previousState[toIdx(key)] && m_currentState[toIdx(key)]; //[cite: 4]
    }

    bool InputManager::isJustKeyPressed(Key k1, Key k2) {
        return isJustKeyPressed(k1) || isJustKeyPressed(k2); //[cite: 4]
    }

    bool InputManager::isJustKeyReleased(Key key) {
        return !m_currentState[toIdx(key)] && m_previousState[toIdx(key)]; //[cite: 4]
    }

    bool InputManager::isJustKeyReleased(Key k1, Key k2) {
        return isJustKeyReleased(k1) || isJustKeyReleased(k2); //[cite: 4]
    }

    // =========================================================
    // ATALHOS COM MODIFICADORES
    // =========================================================

    bool InputManager::isShortcutJustPressed(Key modifier, Key key) {
        return isKeyPressed(modifier) && isJustKeyPressed(key); //[cite: 4]
    }

    bool InputManager::isShortcutReleased(Key modifier, Key key) {
        return isKeyPressed(modifier) && isKeyReleased(key); //[cite: 4]
    }

    // =========================================================
    // MAPEAMENTO DE AÇÕES
    // =========================================================

    void InputManager::addBinding(const std::string& action, KeyBind bind) {
        m_bindings.insert_or_assign(action, bind); //[cite: 4]
    }

    bool InputManager::isActionPressed(const std::string& action) {
        auto it = m_bindings.find(action); //[cite: 4]
        if (it == m_bindings.end()) return false; //[cite: 4]

        const KeyBind& b = it->second; //[cite: 4]
        if (b.modifier != Key::_COUNT) { //[cite: 4]
            return isKeyPressed(b.modifier) && isKeyPressed(b.mainKey); //[cite: 4]
        }
        return isKeyPressed(b.mainKey); //[cite: 4]
    }

    bool InputManager::isActionJustPressed(const std::string& action) {
        auto it = m_bindings.find(action); //[cite: 4]
        if (it == m_bindings.end()) return false; //[cite: 4]

        const KeyBind& b = it->second; //[cite: 4]
        if (b.modifier != Key::_COUNT) { //[cite: 4]
            return isShortcutJustPressed(b.modifier, b.mainKey); //[cite: 4]
        }
        return isJustKeyPressed(b.mainKey); //[cite: 4]
    }

    bool InputManager::isActionJustReleased(const std::string& action) {
        auto it = m_bindings.find(action); //[cite: 4]
        if (it == m_bindings.end()) return false; //[cite: 4]

        const KeyBind& b = it->second; //[cite: 4]
        if (b.modifier != Key::_COUNT) { //[cite: 4]
            return isShortcutReleased(b.modifier, b.mainKey); //[cite: 4]
        }
        return isJustKeyReleased(b.mainKey); //[cite: 4]
    }

    bool InputManager::isActionReleased(const std::string& action) {
        return isActionJustReleased(action); //[cite: 4]
    }

    // =========================================================
    // EIXOS 1D E 2D
    // =========================================================

    float InputManager::getInputValueF(Key k1, Key k2) {
        float input = 0.0f; //[cite: 4]
        if (isKeyPressed(k1)) input -= 1.0f; //[cite: 4]
        if (isKeyPressed(k2)) input += 1.0f; //[cite: 4]
        return input; //[cite: 4]
    }

    float InputManager::getInputValueF(Key k1, Key k2, Key altK1, Key altK2) {
        float input = 0.0f; //[cite: 4]
        if (isKeyPressed(k1) || isKeyPressed(altK1)) input -= 1.0f; //[cite: 4]
        if (isKeyPressed(k2) || isKeyPressed(altK2)) input += 1.0f; //[cite: 4]
        return input; //[cite: 4]
    }

    int InputManager::getInputValue(Key k1, Key k2) {
        int input = 0; //[cite: 4]
        if (isKeyPressed(k1)) input -= 1; //[cite: 4]
        if (isKeyPressed(k2)) input += 1; //[cite: 4]
        return input; //[cite: 4]
    }

    int InputManager::getInputValue(Key k1, Key k2, Key altK1, Key altK2) {
        int input = 0; //[cite: 4]
        if (isKeyPressed(k1) || isKeyPressed(altK1)) input -= 1; //[cite: 4]
        if (isKeyPressed(k2) || isKeyPressed(altK2)) input += 1; //[cite: 4]
        return input; //[cite: 4]
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

    Vec2i InputManager::getInputAxies() {
        return getInputValue2D(Key::H_A, Key::H_D, Key::H_W, Key::H_S,
                               Key::H_LEFT, Key::H_RIGHT, Key::H_UP, Key::H_DOWN); //[cite: 4]
    }

    Vec2f InputManager::getInputAxiesF() {
        return getInputValueF2D(Key::H_A, Key::H_D, Key::H_W, Key::H_S,
                                Key::H_LEFT, Key::H_RIGHT, Key::H_UP, Key::H_DOWN); //[cite: 4]
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
            default: return "UNKNOWN_KEY"; //[cite: 4]
        }
    }

    const char* InputManager::getMouseButtonName(MouseButton button) const {
        switch (button) {
            #define X(name, code) case MouseButton::H_##name: return #name;
            HLX_MOUSE_BUTTON_LIST
            #undef X
            default: return "UNKNOWN_MOUSE_BUTTON"; //[cite: 4]
        }
    }

    void InputManager::update(GLFWwindow* windowHandle) {
        if (!windowHandle) return;

        // 1. Salva o snapshot anterior do teclado[cite: 4]
        m_previousState = m_currentState; //[cite: 4]

        // 2. Consulta de teclado via GLFW Polling
        for (const auto& [hlxKey, glfwKeyCode] : m_keymap) {
            if (glfwKeyCode == GLFW_KEY_UNKNOWN) continue;

            int state = glfwGetKey(windowHandle, glfwKeyCode);
            m_currentState[toIdx(hlxKey)] = (state == GLFW_PRESS || state == GLFW_REPEAT);
        }

        // 3. Salva snapshot anterior do mouse e consulta estado atual via GLFW[cite: 4]
        m_lastMouseState = m_currentMouseState; //[cite: 4]
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

        // 4. Consulta de posição do cursor via GLFW (substituindo SDL_GetMouseState)[cite: 4]
        double cursorX = 0.0;
        double cursorY = 0.0;
        glfwGetCursorPos(windowHandle, &cursorX, &cursorY);

        m_mousePosition = { static_cast<int>(cursorX), static_cast<int>(cursorY) };

        // 5. Cálculo do Delta de movimento do mouse[cite: 4]
        m_mouseDelta = {
            m_mousePosition.first - m_lastMousePos.first,
            m_mousePosition.second - m_lastMousePos.second
        };
        m_lastMousePos = m_mousePosition; //[cite: 4]

        // 6. Lógica de detecção de arrasto (Drag) com botão esquerdo[cite: 4]
        if (isMouseLeftPressed()) { //[cite: 4]
            m_dragStartPosLeft = m_mousePosition; //[cite: 4]
        }

        if (isMouseLeftDown()) { //[cite: 4]
            if (!m_isDraggingLeft) { //[cite: 4]
                if (calculateDistance(m_mousePosition, m_dragStartPosLeft) > static_cast<float>(m_dragThreshold)) { //[cite: 4]
                    m_isDraggingLeft = true; //[cite: 4]
                }
            }
        } else {
            m_isDraggingLeft = false; //[cite: 4]
        }
    }

}