#include <engine/engine.hpp>
#include <iostream>

class EditorApp : public hellix::core::Application {
public:
    EditorApp() : Application("Hellix Editor - Viewport & Debug") {
        std::cout << "[Editor] Camada de inspeção carregada.\n";
    }

    ~EditorApp() override {
        std::cout << "[Editor] Finalizado.\n";
    }

    void onUpdate() override {}
    void onRender() override {}
};

int main() {
    auto app = std::make_unique<EditorApp>();
    app->run();
    return 0;
}