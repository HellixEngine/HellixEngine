#pragma once

#include <engine/engine.hpp>

class ExampleLayer : public hlx::core::Layer {
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void onAttach() override {
        HLX_INFO("ExampleLayer acoplada! Testando configuracao do InputManager...");

        // Registo de acoes e atalhos customizados
        auto& input = hellix::core::input::InputManager::getInstance();
        input.addBinding("Jump", { hellix::core::input::Key::H_SPACE });
        input.addBinding("Save", { hellix::core::input::Key::H_LCTRL, hellix::core::input::Key::H_S });
    }

    void onDetach() override {
        HLX_WARN("ExampleLayer desacoplada!");
    }

    void onUpdate(hlx::TimeStep ts) override {
        auto& input = hlx::Input::getInstance();

        // 1. Validacao de acao com tecla simples (Jump)
        if (input.isActionJustPressed("Jump")) {
            HLX_INFO("[Input Action] 'Jump' disparado via espaco!");
        }

        // 2. Validacao de atalho composto com modificador (CTRL + S)
        if (input.isActionJustPressed("Save")) {
            HLX_INFO("[Input Shortcut] 'Save' acionado (CTRL + S)!");
        }

        // 3. Validacao de eixos direcionais (WASD + Setas)
        auto [axisX, axisY] = input.getInputAxiesF();
        if (axisX != 0.0f || axisY != 0.0f) {
            HLX_TRACE("[Input Axis] Eixo direcional: X = {0}, Y = {1}", axisX, axisY);
        }

        // 4. Validacao de deteccao de arraste de mouse (Drag & Drop)
        if (input.isDraggingLeft()) {
            auto [deltaX, deltaY] = input.getMouseDelta();
            HLX_WARN("[Input Mouse] Arrastando botao esquerdo! Delta: ({0}, {1})", deltaX, deltaY);
        }

        // 5. Validacao de scroll da roda do mouse
        if (input.getMouseWheel() != 0) {
            HLX_TRACE("[Input Mouse] Roda do mouse rolada: {0}", input.getMouseWheel());
        }

        /*if (input.isMouseLeftPressed()) {
            HLX_INFO("[Input Mouse] Botao esquerdo do mouse pressionado!");
        }*/ //ou
        if (input.isMousePressed(hlx::MouseCode::H_BUTTON_LEFT)) {
            HLX_INFO("[Input Mouse] Botao esquerdo do mouse pressionado!");
        }

        if (input.isKeyPressed(hlx::Key::H_R)) {
            HLX_INFO("[Input Key] Tecla R sendo pressionada!");
        }
        if (input.isJustKeyPressed(hlx::Key::H_T)) {
            HLX_INFO("[Input Key] Tecla T acabou de ser pressionada!");
        }

        if (input.isKeyReleased(hlx::Key::H_Y)) {
            HLX_INFO("[Input Key] Tecla Y acabou de ser liberada!");
        }

    }

    void onEvent(hellix::events::Event& event) override {
        // Eventos nativos continuam a fluir pela fila se necessario
    }
};