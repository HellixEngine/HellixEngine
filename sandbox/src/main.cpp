#include <engine/engine.hpp>
#include <iostream>

#include "./example_layer.hpp"


class SandboxApp : public hlx::core::Application {
public:
    SandboxApp() : Application("Hellix Sandbox - Teste de Render 2D") {
        HLX_INFO("Sandbox iniciado com sucesso!");
        pushLayer(new ExampleLayer());
    }

    ~SandboxApp() override {
        HLX_INFO("Sandbox finalizado com sucesso!");
    }


    void onUpdate(hlx::TimeStep ts) override {
        //HLX_INFO("Atualizando o Sandbox. Delta Time: {0} segundos", ts.GetSeconds());

        using namespace hlx;

        if (Input::isKeyPressed(Key::H_W)) {
            HLX_INFO("Tecla W pressionada.");
        }else if (Input::isKeyPressed(Key::H_S)) {
            HLX_INFO("Tecla S pressionada.");
        } if (Input::isKeyPressed(Key::H_A)) {
            HLX_INFO("Tecla A pressionada.");
        } else if (Input::isKeyPressed(Key::H_D)) {
            HLX_INFO("Tecla D pressionada.");
        }
        if (Input::isMouseButtonPressed(Mouse::H_BUTTON_LEFT)) {
            HLX_INFO("Botão esquerdo do mouse pressionado.");
        }else if (Input::isMouseButtonPressed(Mouse::H_BUTTON_RIGHT)) {
            HLX_INFO("Botão direito do mouse pressionado.");
        }
        if (Input::isMouseButtonPressed(Mouse::H_BUTTON_MIDDLE)) {
            HLX_INFO("Botão do meio do mouse pressionado.");
        }

        if (Input::isKeyPressed(Key::H_I)) {
            HLX_INFO("Tecla I pressionada.");
        }
        if (Input::isKeyPressed(Key::H_E)) {
            HLX_ERROR("Tecla E pressionada.");
        }
        if (Input::isKeyPressed(Key::H_Q)) {
            HLX_WARN("Tecla Q pressionada.");
        }
        if (Input::isKeyPressed(Key::H_R)) {
            HLX_TRACE("Tecla R pressionada.");
        }
        if (Input::isKeyPressed(Key::H_C)) {
            HLX_CRITICAL("Tecla C pressionada.");
        }


    }
    void onRender() override {}
};


int main() {
    UTF8_SUPPORT_PT_BR


    auto app = std::make_unique<SandboxApp>();
    app->run();
    return 0;
}