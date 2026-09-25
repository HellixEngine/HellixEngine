#pragma once

#include "engine/client/core.hpp"
#include "engine/client/math.hpp"
#include "engine/client/input.hpp"
#include "engine/client/events.hpp"

class ExampleLayer : public hlx::Layer {
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void onAttach() override {
        HLX_INFO("ExampleLayer acoplada! A iniciar testes do FileSystem (VFS)...");

        // 1. Teste de escrita e leitura de ficheiro de texto via alias assets://
        const std::string testFilePath = "assets://config/test_settings.json";
        const std::string jsonContent = "{\n  \"engine\": \"HellixEngine\",\n  \"version\": 1.0\n}";

        if (hlx::FileSystem::writeText(testFilePath, jsonContent)) {
            HLX_INFO("[FileSystem] Ficheiro de texto escrito com sucesso em: {0}", testFilePath);
        } else {
            HLX_ERROR("[FileSystem] Falha ao escrever ficheiro de texto.");
        }

        std::string loadedText = hlx::FileSystem::readText(testFilePath);
        HLX_TRACE("[FileSystem] Conteudo lido:\n{0}", loadedText);

        // 2. Teste de metadados (tamanho e existencia)
        if (hlx::FileSystem::exists(testFilePath)) {
            uint64_t size = hlx::FileSystem::getFileSize(testFilePath);
            HLX_INFO("[FileSystem] O ficheiro existe no disco! Tamanho: {0} bytes", size);
        }

        // 3. Teste de escrita e leitura binaria com Buffer
        const std::string binFilePath = "assets://data/raw_data.bin";
        hlx::Buffer writeBuffer(4);
        writeBuffer.data[0] = std::byte{0xDE};
        writeBuffer.data[1] = std::byte{0xAD};
        writeBuffer.data[2] = std::byte{0xBE};
        writeBuffer.data[3] = std::byte{0xEF};

        if (hlx::FileSystem::writeBytes(binFilePath, writeBuffer)) {
            HLX_INFO("[FileSystem] Buffer binario gravado com sucesso.");
        }

        hlx::Buffer readBuffer = hlx::FileSystem::readBytes(binFilePath);
        if (readBuffer.isValid()) {
            HLX_INFO("[FileSystem] Buffer binario lido. Tamanho: {0} bytes. Byte 0: 0x{1:X}",
                     readBuffer.size, static_cast<unsigned int>(readBuffer.data[0]));
        }

        // 4. Resolucao de caminho fisico absoluto
        std::filesystem::path physicalPath = hlx::FileSystem::resolve("assets://data/raw_data.bin");
        HLX_TRACE("[FileSystem] Caminho fisico resolvido: {0}", physicalPath.string());


        //teste de integridade de ficheiros essenciais e exibição de mensagem de erro caso não existam
       /* if (!hlx::FileSystem::exists("assets://shaders/basic.glsl")) {
            hlx::Platform::showMessageBox(
                "Erro de Inicialização",
                "Shader base não encontrado. Verifique a integridade dos ficheiros!",
                hlx::MessageBoxType::Error
            );
        }*/

    }

    void onDetach() override {
        HLX_WARN("ExampleLayer desacoplada!");
    }

    void onUpdate(hlx::TimeStep ts) override {
        using namespace hlx;
        using namespace hlx::input;

        auto& input = Input::getInstance();
        if (input.isJustKeyPressed(H_SPACE)) {
            HLX_INFO("[Input] Tecla ESPAÇO pressionada!");
        }
        if (input.isJustKeyPressed(H_ESCAPE)) {
            HLX_INFO("[Input] Tecla ESC pressionada!");
            Application::get().close();
        }

        if (input.isDraggingLeft()) {
            auto dragDelta = input.getMouseDragDelta();
            HLX_INFO("[Input] Arraste do mouse em andamento. Delta: ({0}, {1})", dragDelta.x, dragDelta.y);
        }

    }

    void onEvent(hlx::events::Event& event) override {}
};