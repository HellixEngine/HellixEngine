#include <engine/engine.hpp>
#include <iostream>
#include <engine/events/key_event.hpp>

class SandboxApp : public hellix::core::Application {
public:
    SandboxApp() : Application("Hellix Sandbox - Teste de Render 2D") {
        HLX_INFO("Sandbox iniciado com sucesso!");
    }

    ~SandboxApp() override {
        HLX_INFO("Sandbox finalizado com sucesso!");
    }


    void onUpdate(hlx::TimeStep ts) override {
        HLX_INFO("Atualizando o Sandbox. Delta Time: {0} segundos", ts.GetSeconds());
        HLX_INFO("Pressione a tecla W para ver a mensagem no console.");

        if (hlx::Input::isKeyPressed(hlx::Key::H_W)) {
            HLX_INFO("Tecla W pressionada.");
        }
        if (hlx::Input::isMouseButtonPressed(hlx::Mouse::H_BUTTON_LEFT)) {
            HLX_INFO("Botão esquerdo do mouse pressionado.");
        }


    }
    void onRender() override {}
};

int main() {
    auto app = std::make_unique<SandboxApp>();
    app->run();
    return 0;
}