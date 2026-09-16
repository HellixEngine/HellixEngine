#include <engine/engine.hpp>
#include <iostream>

class SandboxApp : public hellix::core::Application {
public:
    SandboxApp() : Application("Hellix Sandbox - Teste de Render 2D") {
        std::cout << "[Sandbox] Inicializado com sucesso!\n";
    }

    ~SandboxApp() override {
        std::cout << "[Sandbox] Finalizado.\n";
    }

    void onUpdate() override {}
    void onRender() override {}
};

int main() {
    auto app = std::make_unique<SandboxApp>();
    app->run();
    return 0;
}