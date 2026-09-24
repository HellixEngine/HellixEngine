#pragma once

#include <engine/engine.hpp>

class ExampleLayer : public hlx::core::Layer {
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void onAttach() override {
        HLX_WARN("ExampleLayer acoplada! Testando configuracao do InputManager...");

        // Registo de acoes e atalhos customizados
        auto& input = hellix::core::input::InputManager::getInstance();
        input.addBinding("Jump", { hellix::core::input::Key::H_SPACE });
        input.addBinding("Save", { hellix::core::input::Key::H_LCTRL, hellix::core::input::Key::H_S });
    }

    void onDetach() override {
        HLX_WARN("ExampleLayer desacoplada!");
    }


    void onUpdate(hlx::TimeStep ts) override {
        using namespace hlx;
        auto& input = Input::getInstance();
        // 1. Validacao de acao com tecla simples (Jump)
        if (input.isActionJustPressed("Jump")) {
            HLX_INFO("[Input Action] 'Jump' disparado via espaco!");
        }else if (input.isActionReleased("Jump")) {//não faz sentido segurar desacionado kkkkkkkkk, então tanto faz ser isAction(Just)Released
            HLX_INFO("[Input Shortcut] 'Jump' desacionado (Space)!");
        }

        // 2. Validacao de atalho composto com modificador (CTRL + S)
        if (input.isActionJustPressed("Save")) {
            HLX_INFO("[Input Shortcut] 'Save' acionado (CTRL + S)!");
        }

        // 3. Validacao de eixos direcionais (WASD + Setas)
        auto [axisX, axisY] = input.getInputAxesF();
        if ((axisX != 0.0f || axisY != 0.0f) & !input.isKeyPressed(Key::H_LCTRL)) {
            HLX_TRACE("[Input Axis] Eixo direcional: X = {0}, Y = {1}", axisX, axisY);
        }

        // 4. Validacao de deteccao de arraste de mouse (Drag & Drop)
        if (input.isDraggingLeft()) {
            auto [deltaX, deltaY] = input.getMouseDragDelta();
            HLX_WARN("[Input Mouse] Arrastando botao esquerdo! Delta: ({0}, {1})", deltaX, deltaY);

        }

        // 5. Validacao de scroll da roda do mouse
        if (input.getMouseWheel() != 0) {
            HLX_TRACE("[Input Mouse] Roda do mouse rolada: {0}", input.getMouseWheel());
        }

        /*if (input.isMouseLeftPressed()) {
            HLX_INFO("[Input Mouse] Botao esquerdo do mouse pressionado!");
        }*/
        //ou
        if (input.isMousePressed(MouseCode::H_BUTTON_LEFT)) {// outra forma de chamar o metodo, mas serve para todos os botoes do mouse, inclusive o scroll click (middle button)
            HLX_INFO("[Input Mouse] Botao esquerdo do mouse pressionado!");
        }
        // 6. Validacao de teclas individuais (R, T, Y)
        if (input.isKeyPressed(Key::H_R)) {
            HLX_INFO("[Input Key] Tecla R sendo pressionada!");
        }
        if (input.isJustKeyPressed(Key::H_T)) {
            HLX_INFO("[Input Key] Tecla T acabou de ser pressionada!");
        }
        if (input.isKeyReleased(Key::H_Y)) {// ou isJustKeyReleased(Key::H_Y) o resultado é o mesmo
            HLX_INFO("[Input Key] Tecla Y acabou de ser liberada!");
        }
    }

    void onEvent(hlx::events::Event& event) override {
        // Eventos nativos continuam a fluir pela fila se necessario
    }
};