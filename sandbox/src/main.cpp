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

    }
    void onRender() override {}
    void onEvent(hellix::events::Event &e) {
        Application::onEvent(e);
        auto dispatcher = hellix::events::EventDispatcher(e);

        dispatcher.dispatch<hellix::events::KeyPressedEvent>([this](hellix::events::KeyPressedEvent& event) {
            if (event.getKeyCode() == static_cast<int>(hellix::events::Keys::H_W)) {
                std::cout << "[Sandbox] Tecla W pressionada. Encerrando aplicação...\n";
            }
            return true;
        });
    }
};

int main() {
    auto app = std::make_unique<SandboxApp>();
    app->run();
    return 0;
}