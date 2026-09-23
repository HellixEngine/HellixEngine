#pragma once

#include <engine/engine.hpp>
#

class ExampleLayer : public hlx::core::Layer {
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void onAttach() override {
        HLX_INFO("ExampleLayer acoplada com sucesso!");
    }

    void onDetach() override {
        HLX_WARN("ExampleLayer desacoplada!");
    }

    void onUpdate(hlx::TimeStep ts) override {
        // Log leve ou atualização periódica de teste
        //HLX_TRACE("Frame time: {0}ms", ts.GetMilliseconds());
    }

    void onEvent(hlx::events::Event& event) override {
        // Exemplo: tratar tecla pressionada
        /*hlx::events::EventDispatcher dispatcher(event);
        dispatcher.dispatch<hlx::events::KeyPressedEvent>(
            [this](hlx::events::KeyPressedEvent& e) {
                return onKeyPressed(e);
            }
        );*/
    }

private:
    /*bool onKeyPressed(hlx::events::KeyPressedEvent& e) {
        HLX_TRACE("Tecla capturada na camada: {0}", e.getKeyCode());
        return false; // Retorna true se quiser consumir o evento e impedir propagação
    }*/
};