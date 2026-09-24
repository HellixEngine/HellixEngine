#include <engine/client/core.hpp>
#include <iostream>

#include "./example_layer.hpp"


class SandboxApp : public hlx::Application {
public:
    SandboxApp() : Application("Hellix Sandbox - Teste de Render 2D") {
        HLX_INFO("Sandbox iniciado com sucesso!");
        pushLayer(new ExampleLayer());


        auto iconPath = hlx::FileSystem::resolve("assets://icon/icon.png");



        getWindow().setIcon(iconPath.string().c_str());

    }

    ~SandboxApp() override {
        HLX_INFO("Sandbox finalizado com sucesso!");
    }


    void onUpdate(hlx::TimeStep ts) override {}
    void onRender() override {}
};


int main() {
    UTF8_SUPPORT_PT_BR


    auto app = std::make_unique<SandboxApp>();
    app->run();
    return 0;
}