#pragma once

#include <cstdint>
#include <string_view>
#include "engine/core/buffer.hpp"

namespace hellix::core {

    /**
     * @brief Estrutura RAII para carregamento e manuseio de dados de pixel via STB.
     */
    struct ImageData {
        /** @brief Dados de pixel carregados. */
        uint8_t* pixels = nullptr;
        /** @brief Largura da imagem em pixels. */
        int width = 0;
        /** @brief Altura da imagem em pixels. */
        int height = 0;
        /** @brief Quantidade de canais por pixel. */
        int channels = 0;

        ImageData() = default;

        ~ImageData() {
            release();
        }

        // Semântica de movimento.
        ImageData(ImageData&& other) noexcept
            : pixels(other.pixels), width(other.width), height(other.height), channels(other.channels) {
            other.pixels = nullptr;
            other.width = 0;
            other.height = 0;
            other.channels = 0;
        }

        ImageData& operator=(ImageData&& other) noexcept {
            if (this != &other) {
                release();
                pixels = other.pixels;
                width = other.width;
                height = other.height;
                channels = other.channels;

                other.pixels = nullptr;
                other.width = 0;
                other.height = 0;
                other.channels = 0;
            }
            return *this;
        }

        // Proíbe cópias automáticas pesadas
        ImageData(const ImageData&) = delete;
        ImageData& operator=(const ImageData&) = delete;

        /** @brief Libera os pixels carregados e zera os metadados. */
        void release();

        /** @brief Informa se os pixels e as dimensões da imagem são válidos. */
        [[nodiscard]] bool isValid() const noexcept {
            return pixels != nullptr && width > 0 && height > 0;
        }

        /** @brief Retorna o tamanho estimado dos pixels em bytes. */
        [[nodiscard]] uint64_t getSizeInBytes() const noexcept {
            return static_cast<uint64_t>(width) * height * channels;
        }
    };

    /**
     * @brief Subsistema/Utilitário para manipulação de texturas e imagens.
     */
    class ImageLoader {
    public:
        /**
         * @brief Carrega uma imagem a partir de um caminho virtual da engine (ex: "assets://textures/wall.png").
         * @param virtualPath Caminho resolvido pelo VFS.
         * @param flipVertically Inverte os eixos Y (essencial para texturas OpenGL).
         * @param desiredChannels Força canais específicos (ex: 4 para RGBA, 0 para manter original).
         */
        static ImageData load(std::string_view virtualPath, bool flipVertically = true, int desiredChannels = 4);

        /**
         * @brief Carrega uma imagem contida em memória dentro de um Buffer.
         */
        static ImageData loadFromMemory(const Buffer& buffer, bool flipVertically = true, int desiredChannels = 4);

        /**
         * @brief Salva dados de pixel em formato PNG no disco através do VFS.
         */
        static bool savePNG(std::string_view virtualPath, int width, int height, int channels, const void* data);
    };

}

namespace hlx {
    using ImageData = hellix::core::ImageData;
    using ImageLoader = hellix::core::ImageLoader;
}