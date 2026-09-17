#include <engine/engine.hpp>
#include <iostream>
#include <engine/events/key_event.hpp>

class SandboxApp : public hellix::core::Application {
public:
    SandboxApp() : Application("Hellix Sandbox - Teste de Render 2D") {
        std::cout << "[Sandbox] Inicializado com sucesso!\n";
    }

    ~SandboxApp() override {
        std::cout << "[Sandbox] Finalizado.\n";
    }


    void onUpdate() override {

        if (hlx::Input::isKeyPressed(hlx::Key::H_W)) {
            std::cout << "[Sandbox] Tecla W pressionada.\n";
        }
        if (hlx::Input::isMouseButtonPressed(hlx::Mouse::H_BUTTON_LEFT)) {
            std::cout << "[Sandbox] Botão esquerdo do mouse pressionado.\n";
        }

    }
    void onRender() override {}
};

int main() {
    auto app = std::make_unique<SandboxApp>();
    app->run();
    return 0;
}