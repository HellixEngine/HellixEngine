#pragma once

#include <bitset>
#include <string>
#include <unordered_map>
#include <utility>
#include <GLFW/glfw3.h>

/**
 * @file input_manager.hpp
 * @brief Subsistema de entrada e gerenciamento de teclado/mouse para a HellixEngine.
 */

/**
 * @brief Lista X-Macro para mapeamento de teclas HellixEngine para os códigos do GLFW.
 * @details Formato: X(NomeSemPrefixo, ConstanteGLFW).
 * O enumerador recebe o prefixo H_ automaticamente, enquanto a stringificação expõe o nome limpo.
 */
#define HLX_KEY_LIST \
    X(UNKNOWN, GLFW_KEY_UNKNOWN) \
    X(A, GLFW_KEY_A) X(B, GLFW_KEY_B) X(C, GLFW_KEY_C) X(D, GLFW_KEY_D) \
    X(E, GLFW_KEY_E) X(F, GLFW_KEY_F) X(G, GLFW_KEY_G) X(H, GLFW_KEY_H) \
    X(I, GLFW_KEY_I) X(J, GLFW_KEY_J) X(K, GLFW_KEY_K) X(L, GLFW_KEY_L) \
    X(M, GLFW_KEY_M) X(N, GLFW_KEY_N) X(O, GLFW_KEY_O) X(P, GLFW_KEY_P) \
    X(Q, GLFW_KEY_Q) X(R, GLFW_KEY_R) X(S, GLFW_KEY_S) X(T, GLFW_KEY_T) \
    X(U, GLFW_KEY_U) X(V, GLFW_KEY_V) X(W, GLFW_KEY_W) X(X, GLFW_KEY_X) \
    X(Y, GLFW_KEY_Y) X(Z, GLFW_KEY_Z) \
    X(N0, GLFW_KEY_0) X(N1, GLFW_KEY_1) X(N2, GLFW_KEY_2) X(N3, GLFW_KEY_3) \
    X(N4, GLFW_KEY_4) X(N5, GLFW_KEY_5) X(N6, GLFW_KEY_6) X(N7, GLFW_KEY_7) \
    X(N8, GLFW_KEY_8) X(N9, GLFW_KEY_9) \
    X(RETURN, GLFW_KEY_ENTER) X(ESCAPE, GLFW_KEY_ESCAPE) \
    X(BACKSPACE, GLFW_KEY_BACKSPACE) X(TAB, GLFW_KEY_TAB) \
    X(SPACE, GLFW_KEY_SPACE) \
    X(MINUS, GLFW_KEY_MINUS) X(EQUALS, GLFW_KEY_EQUAL) \
    X(LEFTBRACKET, GLFW_KEY_LEFT_BRACKET) X(RIGHTBRACKET, GLFW_KEY_RIGHT_BRACKET) \
    X(BACKSLASH, GLFW_KEY_BACKSLASH) X(SEMICOLON, GLFW_KEY_SEMICOLON) \
    X(APOSTROPHE, GLFW_KEY_APOSTROPHE) X(GRAVE, GLFW_KEY_GRAVE_ACCENT) \
    X(COMMA, GLFW_KEY_COMMA) X(PERIOD, GLFW_KEY_PERIOD) \
    X(SLASH, GLFW_KEY_SLASH) \
    X(F1, GLFW_KEY_F1) X(F2, GLFW_KEY_F2) X(F3, GLFW_KEY_F3) X(F4, GLFW_KEY_F4) \
    X(F5, GLFW_KEY_F5) X(F6, GLFW_KEY_F6) X(F7, GLFW_KEY_F7) X(F8, GLFW_KEY_F8) \
    X(F9, GLFW_KEY_F9) X(F10, GLFW_KEY_F10) X(F11, GLFW_KEY_F11) X(F12, GLFW_KEY_F12) \
    X(F13, GLFW_KEY_F13) X(F14, GLFW_KEY_F14) X(F15, GLFW_KEY_F15) X(F16, GLFW_KEY_F16) \
    X(F17, GLFW_KEY_F17) X(F18, GLFW_KEY_F18) X(F19, GLFW_KEY_F19) X(F20, GLFW_KEY_F20) \
    X(F21, GLFW_KEY_F21) X(F22, GLFW_KEY_F22) X(F23, GLFW_KEY_F23) X(F24, GLFW_KEY_F24) \
    X(INSERT, GLFW_KEY_INSERT) X(HOME, GLFW_KEY_HOME) \
    X(PAGEUP, GLFW_KEY_PAGE_UP) X(DELETE, GLFW_KEY_DELETE) \
    X(END, GLFW_KEY_END) X(PAGEDOWN, GLFW_KEY_PAGE_DOWN) \
    X(RIGHT, GLFW_KEY_RIGHT) X(LEFT, GLFW_KEY_LEFT) \
    X(DOWN, GLFW_KEY_DOWN) X(UP, GLFW_KEY_UP) \
    X(KP_0, GLFW_KEY_KP_0) X(KP_1, GLFW_KEY_KP_1) \
    X(KP_2, GLFW_KEY_KP_2) X(KP_3, GLFW_KEY_KP_3) \
    X(KP_4, GLFW_KEY_KP_4) X(KP_5, GLFW_KEY_KP_5) \
    X(KP_6, GLFW_KEY_KP_6) X(KP_7, GLFW_KEY_KP_7) \
    X(KP_8, GLFW_KEY_KP_8) X(KP_9, GLFW_KEY_KP_9) \
    X(KP_PERIOD, GLFW_KEY_KP_DECIMAL) X(KP_PLUS, GLFW_KEY_KP_ADD) \
    X(KP_MINUS, GLFW_KEY_KP_SUBTRACT) X(KP_MULTIPLY, GLFW_KEY_KP_MULTIPLY) \
    X(KP_DIVIDE, GLFW_KEY_KP_DIVIDE) X(KP_ENTER, GLFW_KEY_KP_ENTER) \
    X(LCTRL, GLFW_KEY_LEFT_CONTROL) X(LSHIFT, GLFW_KEY_LEFT_SHIFT) \
    X(LALT, GLFW_KEY_LEFT_ALT) X(LGUI, GLFW_KEY_LEFT_SUPER) \
    X(RCTRL, GLFW_KEY_RIGHT_CONTROL) X(RSHIFT, GLFW_KEY_RIGHT_SHIFT) \
    X(RALT, GLFW_KEY_RIGHT_ALT) X(RGUI, GLFW_KEY_RIGHT_SUPER) \
    X(CAPSLOCK, GLFW_KEY_CAPS_LOCK) X(SCROLLLOCK, GLFW_KEY_SCROLL_LOCK) \
    X(NUMLOCKCLEAR, GLFW_KEY_NUM_LOCK) \
    X(PRINTSCREEN, GLFW_KEY_PRINT_SCREEN) X(PAUSE, GLFW_KEY_PAUSE)


#define HLX_MOUSE_BUTTON_LIST \
    X(BUTTON_LEFT, GLFW_MOUSE_BUTTON_LEFT) \
    X(BUTTON_RIGHT, GLFW_MOUSE_BUTTON_RIGHT) \
    X(BUTTON_MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE) \
    X(BUTTON_1, GLFW_MOUSE_BUTTON_1) \
    X(BUTTON_2, GLFW_MOUSE_BUTTON_2)\
    X(BUTTON_3, GLFW_MOUSE_BUTTON_3)\
    X(BUTTON_4, GLFW_MOUSE_BUTTON_4) \
    X(BUTTON_5, GLFW_MOUSE_BUTTON_5) \
    X(BUTTON_6, GLFW_MOUSE_BUTTON_6) \
    X(BUTTON_7, GLFW_MOUSE_BUTTON_7) \
    X(BUTTON_8, GLFW_MOUSE_BUTTON_8)

namespace hellix::core::input {

    /**
     * @brief Alias temporário para vetores 2D inteiros.
     * @note todo_: Substituir pelo tipo definitivo da biblioteca de matemática da HellixEngine (ex: Vec2i).
     */
    using Vec2i = std::pair<int, int>;

    /**
     * @brief Alias temporário para vetores 2D de ponto flutuante.
     * @note todo_: Substituir pelo tipo definitivo da biblioteca de matemática da HellixEngine (ex: Vec2f).
     */
    using Vec2f = std::pair<float, float>;

    /**
     * @brief Enumeração fortemente tipada para as teclas suportadas pela engine.
     * @details Cada identificador possui o prefixo H_ gerado via X-Macro.
     */
    enum class Key {
        #define X(name, code) H_##name,
        HLX_KEY_LIST
        #undef X
        _COUNT
    };

    /**
     * @brief Enumeração fortemente tipada para os botões do mouse suportados pela engine.
     * @details Cada identificador possui o prefixo H_ gerado via X-Macro.
     */
    enum class MouseButton {
        #define X(name, code) H_##name,
        HLX_MOUSE_BUTTON_LIST
        #undef X
        _COUNT
    };

    /**
     * @brief Estrutura para definição de vínculos de ação (KeyBinds) com suporte a modificador.
     */
    struct KeyBind {
        Key mainKey;                 ///< Tecla principal que aciona a ação.
        Key modifier = Key::_COUNT;  ///< Tecla modificadora opcional (ex: CTRL, SHIFT). O valor _COUNT representa ausência de modificador.

        /** @brief Construtor padrão. */
        KeyBind() : mainKey(Key::H_UNKNOWN) {}

        /**
         * @brief Cria um vínculo de tecla simples.
         * @param k Tecla a ser vinculada.
         */
        KeyBind(Key k) : mainKey(k) {}

        /**
         * @brief Cria um vínculo de tecla composto com modificador.
         * @param mod Tecla modificadora (ex: Key::H_LCTRL).
         * @param k Tecla principal que dispara a ação.
         */
        KeyBind(Key mod, Key k) : mainKey(k), modifier(mod) {}
    };

    /**
     * @brief Gerenciador central de entradas de teclado e mouse da HellixEngine.
     * @details Implementa o padrão Singleton e gerencia estados de transição (just pressed/released),
     * arraste de mouse (drag), eixos direcionais e despacho de atalhos.
     */
    class InputManager {
    public:
        /**
         * @brief Obtém a referência estática para a instância singleton.
         * @return Referência para o InputManager.
         */
        static InputManager& getInstance();

        // =========================================================
        // TECLADO: CONSULTAS CONTÍNUAS E TRANSIÇÕES
        // =========================================================

        /**
         * @brief Verifica se uma tecla está sendo segurada no frame atual.
         * @param key Tecla a consultar.
         * @return True se a tecla estiver pressionada.
         */
        bool isKeyPressed(Key key);

        /**
         * @brief Verifica se ao menos uma de duas teclas está sendo segurada.
         * @param k1 Primeira tecla.
         * @param k2 Segunda tecla.
         * @return True se k1 ou k2 estiverem pressionadas.
         */
        bool isKeyPressed(Key k1, Key k2);

        /**
         * @brief Verifica se uma tecla foi solta (transição para liberada).
         * @param key Tecla a consultar.
         * @return True se a tecla transicionou de pressionada para liberada.
         */
        bool isKeyReleased(Key key);

        /**
         * @brief Verifica se ao menos uma de duas teclas foi solta.
         * @param k1 Primeira tecla.
         * @param k2 Segunda tecla.
         * @return True se k1 ou k2 transicionaram para liberadas.
         */
        bool isKeyReleased(Key k1, Key k2);

        /**
         * @brief Verifica se a tecla foi recém-pressionada neste frame (borda de subida).
         * @param key Tecla a consultar.
         * @return True apenas no frame exato do pressionamento.
         */
        bool isJustKeyPressed(Key key);

        /**
         * @brief Verifica se ao menos uma de duas teclas foi recém-pressionada neste frame.
         * @param k1 Primeira tecla.
         * @param k2 Segunda tecla.
         * @return True se k1 ou k2 foram recém-pressionadas.
         */
        bool isJustKeyPressed(Key k1, Key k2);

        /**
         * @brief Verifica se a tecla foi recém-liberada neste frame (borda de descida).
         * @param key Tecla a consultar.
         * @return True apenas no frame exato da liberação.
         */
        bool isJustKeyReleased(Key key);

        /**
         * @brief Verifica se ao menos uma de duas teclas foi recém-liberada neste frame.
         * @param k1 Primeira tecla.
         * @param k2 Segunda tecla.
         * @return True se k1 ou k2 foram recém-liberadas.
         */
        bool isJustKeyReleased(Key k1, Key k2);

        /**
         * @brief Verifica se todas as teclas fornecidas estão simultaneamente seguradas.
         * @tparam Args Conjunto de tipos Key.
         * @param keys Teclas a serem validadas.
         * @return True se absolutamente todas as teclas estiverem pressionadas.
         */
        template<typename... Args>
        bool areAllKeysPressed(Args... keys) {
            return (isKeyPressed(keys) && ...);
        }

        // =========================================================
        // ATALHOS COM MODIFICADORES
        // =========================================================

        /**
         * @brief Verifica se um atalho foi acionado (Modificador segurado + Tecla recém-pressionada).
         * @param modifier Tecla modificadora exigida (ex: Key::H_LCTRL).
         * @param key Tecla principal disparadora.
         * @return True no frame em que a tecla principal é pressionada com o modificador ativo.
         */
        bool isShortcutJustPressed(Key modifier, Key key);

        /**
         * @brief Verifica se um atalho foi finalizado (Modificador ainda ativo + Tecla recém-solta).
         * @param modifier Tecla modificadora exigida.
         * @param key Tecla principal liberada.
         * @return True no frame em que a tecla principal é solta com o modificador ativo.
         */
        bool isShortcutReleased(Key modifier, Key key);

        // =========================================================
        // MAPEAMENTO DE AÇÕES
        // =========================================================

        /**
         * @brief Registra um vínculo nomeado para uma ação customizável.
         * @param action Identificador textual da ação (ex: "Jump", "Fire").
         * @param bind Configuração de tecla e modificador associada.
         */
        void addBinding(const std::string& action, KeyBind bind);


        /**
         * @brief Registra um vínculo nomeado para uma ação customizável.
         * @param action Identificador textual da ação (ex: "Jump", "Fire").
         * @param bind Configuração de tecla e modificador associada.
         * @return Retorna o nome da ação registrada.
         */
        std::string addBindingR(const std::string& action, KeyBind bind);

        /**
         * @brief Verifica se a ação mapeada está sendo mantida pressionada.
         * @param action Nome da ação.
         * @return True se os requisitos da ação estiverem ativos no frame.
         */
        bool isActionPressed(const std::string& action);

        /**
         * @brief Verifica se a ação mapeada foi recém-acionada neste frame.
         * @param action Nome da ação.
         * @return True no frame de ativação da ação.
         */
        bool isActionJustPressed(const std::string& action);

        /**
         * @brief Verifica se a ação mapeada foi recém-solta neste frame.
         * @param action Nome da ação.
         * @return True no frame de liberação da ação.
         */
        bool isActionJustReleased(const std::string& action);

        /**
         * @brief Alias conveniente para isActionJustReleased.
         * @param action Nome da ação.
         * @return True no frame de liberação da ação.
         */
        bool isActionReleased(const std::string& action);

        // =========================================================
        // EIXOS 1D E 2D (ENTRADAS DIRECIONAIS)
        // =========================================================

        /**
         * @brief Calcula um eixo unidimensional contínuo entre [-1.0f, 1.0f].
         * @param k1 Tecla mapeada para direção negativa (-1.0f).
         * @param k2 Tecla mapeada para direção positiva (+1.0f).
         * @return Valor float resultante da soma das entradas.
         */
        float getInputValueF(Key k1, Key k2);

        /**
         * @brief Calcula um eixo unidimensional contínuo considerando teclas principais e alternativas.
         * @param k1 Tecla primária negativa.
         * @param k2 Tecla primária positiva.
         * @param altK1 Tecla alternativa negativa.
         * @param altK2 Tecla alternativa positiva.
         * @return Valor float no intervalo [-1.0f, 1.0f].
         */
        float getInputValueF(Key k1, Key k2, Key altK1, Key altK2);

        /**
         * @brief Calcula um eixo unidimensional discreto inteiro entre [-1, 1].
         * @param k1 Tecla mapeada para direção negativa (-1).
         * @param k2 Tecla mapeada para direção positiva (+1).
         * @return Inteiro resultante da soma das entradas.
         */
        int getInputValue(Key k1, Key k2);

        /**
         * @brief Calcula um eixo unidimensional discreto considerando teclas principais e alternativas.
         * @param k1 Tecla primária negativa.
         * @param k2 Tecla primária positiva.
         * @param altK1 Tecla alternativa negativa.
         * @param altK2 Tecla alternativa positiva.
         * @return Inteiro no intervalo [-1, 1].
         */
        int getInputValue(Key k1, Key k2, Key altK1, Key altK2);

        /**
         * @brief Monta um vetor 2D discreto inteiro a partir de quatro teclas direcionais.
         * @param left Tecla negativa no eixo X.
         * @param right Tecla positiva no eixo X.
         * @param up Tecla positiva no eixo Y.
         * @param down Tecla negativa no eixo Y.
         * @return Par ordenado com os eixos calculados.
         */
        Vec2i getInputValue2D(Key left, Key right, Key up, Key down);

        /**
         * @brief Monta um vetor 2D discreto inteiro com conjunto primário e alternativo de teclas.
         * @param left Tecla primária X-.
         * @param right Tecla primária X+.
         * @param up Tecla primária Y+.
         * @param down Tecla primária Y-.
         * @param altLeft Tecla alternativa X-.
         * @param altRight Tecla alternativa X+.
         * @param altUp Tecla alternativa Y+.
         * @param altDown Tecla alternativa Y-.
         * @return Par ordenado inteiro representativo do eixo 2D.
         */
        Vec2i getInputValue2D(Key left, Key right, Key up, Key down, Key altLeft, Key altRight, Key altUp, Key altDown);

        /**
         * @brief Monta um vetor 2D de ponto flutuante a partir de quatro teclas direcionais.
         * @param left Tecla negativa no eixo X.
         * @param right Tecla positiva no eixo X.
         * @param up Tecla positiva no eixo Y.
         * @param down Tecla negativa no eixo Y.
         * @return Par ordenado float representativo do eixo 2D.
         */
        Vec2f getInputValueF2D(Key left, Key right, Key up, Key down);

        /**
         * @brief Monta um vetor 2D de ponto flutuante com conjunto primário e alternativo de teclas.
         * @param left Tecla primária X-.
         * @param right Tecla primária X+.
         * @param up Tecla primária Y+.
         * @param down Tecla primária Y-.
         * @param altLeft Tecla alternativa X-.
         * @param altRight Tecla alternativa X+.
         * @param altUp Tecla alternativa Y+.
         * @param altDown Tecla alternativa Y-.
         * @return Par ordenado float representativo do eixo 2D.
         */
        Vec2f getInputValueF2D(Key left, Key right, Key up, Key down, Key altLeft, Key altRight, Key altUp, Key altDown);

        /**
         * @brief Retorna os eixos direcionais discretos padrão da engine (WASD + Setas direcionais).
         * @return Vetor 2D com eixos em coordenadas inteiras.
         */
        Vec2i getInputAxes();

        /**
         * @brief Retorna os eixos direcionais contínuos padrão da engine (WASD + Setas direcionais).
         * @return Vetor 2D com eixos normalizáveis em ponto flutuante.
         */
        Vec2f getInputAxesF();

        // =========================================================
        // MOUSE: POSIÇÃO, DELTA E SCROLL
        // =========================================================

        /**
         * @brief Retorna a posição atual do cursor na janela em pixels.
         * @return Coordenadas X e Y atuais do mouse.
         */
        [[nodiscard]] Vec2i getMousePosition() const { return m_mousePosition; }

        /**
         * @brief Retorna a variação (deslocamento) do mouse ocorrida neste frame.
         * @return Vetor delta (posição atual subtraída da posição do último frame).
         */
        [[nodiscard]] Vec2i getMouseDelta() const { return m_mouseDelta; }

        /**
         * @brief Retorna o deslocamento do arraste do mouse (drag) desde o início do clique.
         * @return Vetor de deslocamento do arraste.
         */
        [[nodiscard]] Vec2i getMouseDragDelta() const { return m_dragDelta; }

        /**
         * @brief Informa se o botão esquerdo do mouse está realizando uma operação de arrasto (drag).
         * @return True se a distância desde o clique inicial ultrapassou o limiar de arrasto.
         */
        [[nodiscard]] bool isDraggingLeft() const { return m_isDraggingLeft; }

        /**
         * @brief Define a distância mínima em pixels necessária para considerar o clique como início de arrasto.
         * @param threshold Quantidade mínima de deslocamento em pixels.
         */
        void setDragThreshold(int threshold) { m_dragThreshold = threshold; }

        /**
         * @brief Define o valor acumulado da roda de rolagem do mouse (wheel).
         * @param y Deslocamento vertical da roda informado pela camada de eventos do GLFW.
         */
        void setMouseWheel(int y) { m_mouseWheelY = y; }

        /**
         * @brief Retorna o valor de rolagem da roda do mouse no frame atual.
         * @return Deslocamento vertical da rolagem (-1, 0 ou 1).
         */
        [[nodiscard]] int getMouseWheel() const { return m_mouseWheelY; }

        /**
         * @brief Reseta o deslocamento da roda do mouse para zero.
         */
        void resetMouseWheel() { m_mouseWheelY = 0; }

        // =========================================================
        // MOUSE: TRANSIÇÕES DE BOTÕES (JUST PRESSED / RELEASED)
        // =========================================================

        /** @brief Informa se o botão do mouse foi recém-pressionado neste frame. */
        [[nodiscard]] bool isMousePressed(MouseButton button) const;

        /** @brief Informa se o botão esquerdo do mouse foi recém-pressionado neste frame. */
        [[nodiscard]] bool isMouseLeftPressed() const;

        /** @brief Informa se o botão do mouse foi recém-liberado neste frame. */
        [[nodiscard]] bool isMouseReleased(MouseButton button) const;

        /** @brief Informa se o botão esquerdo do mouse foi recém-liberado neste frame. */
        [[nodiscard]] bool isMouseLeftReleased() const;

        /** @brief Informa se o botão direito do mouse foi recém-pressionado neste frame. */
        [[nodiscard]] bool isMouseRightPressed() const;

        /** @brief Informa se o botão direito do mouse foi recém-liberado neste frame. */
        [[nodiscard]] bool isMouseRightReleased() const;

        /** @brief Informa se o botão do meio (scroll click) foi recém-pressionado neste frame. */
        [[nodiscard]] bool isMouseMiddlePressed() const;

        /** @brief Informa se o botão do meio (scroll click) foi recém-liberado neste frame. */
        [[nodiscard]] bool isMouseMiddleReleased() const;

        // =========================================================
        // MOUSE: ESTADO CONTÍNUO (DOWN / HELD)
        // =========================================================

        /** @brief Informa se o botão do mouse está sendo mantido pressionado. */
        [[nodiscard]] bool isMouseButtonDown(MouseButton button) const;

        /** @brief Informa se o botão esquerdo do mouse está sendo mantido pressionado. */
        [[nodiscard]] bool isMouseLeftDown() const;

        /** @brief Informa se o botão direito do mouse está sendo mantido pressionado. */
        [[nodiscard]] bool isMouseRightDown() const;

        /** @brief Informa se o botão do meio do mouse está sendo mantido pressionado. */
        [[nodiscard]] bool isMouseMiddleDown() const;

        // =========================================================
        // REFLEXÃO DE NOMES E CICLO DE ATUALIZAÇÃO
        // =========================================================

        /**
         * @brief Obtém a representação textual legível de uma tecla sem o prefixo H_.
         * @param key Tecla desejada.
         * @return Nome literal da tecla em formato de string C constante.
         */
        [[nodiscard]] const char* getKeyName(Key key) const;

        /**
         * @brief Obtém a representação textual legível de um botão do mouse sem o prefixo H_.
         * @param button Botão desejado.
         * @return Nome literal do botão em formato de string C constante.
         */
        [[nodiscard]] const char* getMouseButtonName(MouseButton button) const;

        /**
         * @brief Sincroniza o estado de todos os botões e eixos com base na janela informada.
         * @details Deve ser executado uma vez por frame antes da lógica de atualização das camadas (Layers).
         * @param windowHandle Ponteiro bruto para a janela GLFW em execução.
         */
        void update(GLFWwindow* windowHandle);

    private:
        static constexpr int NUM_KEYS = static_cast<int>(Key::_COUNT);

        std::bitset<NUM_KEYS> m_currentState{};   ///< Tabela de bits com o estado do teclado no frame corrente.
        std::bitset<NUM_KEYS> m_previousState{};  ///< Tabela de bits com o snapshot do frame anterior para detecção de transições.
        std::unordered_map<Key, int> m_keymap;    ///< Tabela de correspondência entre o enum Key e os códigos GLFW.

        Vec2i m_mousePosition{0, 0};              ///< Posição absoluta do cursor no frame corrente.
        Vec2i m_lastMousePos{0, 0};               ///< Posição do cursor gravada no último frame.
        Vec2i m_mouseDelta{0, 0};                 ///< Vetor de deslocamento relativo do mouse neste frame.
        Vec2i m_dragDelta{0, 0};                  ///< Vetor de deslocamento do arraste do mouse (drag) desde o início do clique.
        Vec2i m_dragStartPosLeft{0, 0};           ///< Coordenadas de início do clique para cálculo de arraste.
        int m_mouseWheelY = 0;                    ///< Valor de rolagem vertical da roda do mouse.
        int m_dragThreshold = 4;                  ///< Limiar mínimo em pixels para ativação do arraste.
        bool m_isDraggingLeft = false;            ///< Flag indicativa se o arraste do botão esquerdo está em andamento.

        uint32_t m_currentMouseState = 0;         ///< Máscara de bits dos botões do mouse no frame corrente.
        uint32_t m_lastMouseState = 0;            ///< Máscara de bits dos botões do mouse gravada no último frame.

        std::unordered_map<std::string, KeyBind> m_bindings; ///< Dicionário de ações vinculadas.

        /**
         * @brief Converte o enumerador Key para índice inteiro sem overhead.
         * @param key Enumerador da tecla.
         * @return Valor numérico posicional da tecla.
         */
        int toIdx(Key key) const { return static_cast<int>(key); }

        InputManager();
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
    };

}