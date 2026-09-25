#include "engine/core/image.hpp"
#include "engine/core/file_system.hpp"
#include "engine/core/log.hpp"

#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

namespace hellix::core {

    void ImageData::release() {
        if (pixels) {
            stbi_image_free(pixels);
            pixels = nullptr;
        }
        width = 0;
        height = 0;
        channels = 0;
    }

    ImageData ImageLoader::load(std::string_view virtualPath, bool flipVertically, int desiredChannels) {
        // Resolve o caminho físico pelo VFS
        std::filesystem::path physicalPath = FileSystem::resolve(virtualPath);

        stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

        ImageData image;
        image.pixels = stbi_load(
            physicalPath.string().c_str(),
            &image.width,
            &image.height,
            &image.channels,
            desiredChannels
        );

        if (!image.pixels) {
            HELLIX_ERROR("ImageLoader: Falha ao carregar textura: {0}. Razao: {1}", 
                              virtualPath, stbi_failure_reason());
            return ImageData{};
        }

        if (desiredChannels != 0) {
            image.channels = desiredChannels;
        }

        return image;
    }

    ImageData ImageLoader::loadFromMemory(const Buffer& buffer, bool flipVertically, int desiredChannels) {
        if (!buffer.isValid()) {
            HELLIX_ERROR("ImageLoader: Buffer de memoria invalido.");
            return ImageData{};
        }

        stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

        ImageData image;
        image.pixels = stbi_load_from_memory(
            buffer.as<const unsigned char>(),
            static_cast<int>(buffer.size),
            &image.width,
            &image.height,
            &image.channels,
            desiredChannels
        );

        if (!image.pixels) {
            HELLIX_ERROR("ImageLoader: Falha ao decodificar imagem da memoria. Razao: {0}", 
                              stbi_failure_reason());
            return ImageData{};
        }

        if (desiredChannels != 0) {
            image.channels = desiredChannels;
        }

        return image;
    }

    bool ImageLoader::savePNG(std::string_view virtualPath, int width, int height, int channels, const void* data) {
        std::filesystem::path physicalPath = FileSystem::resolve(virtualPath);

        // Garante que o diretório pai existe
        std::error_code ec;
        if (physicalPath.has_parent_path()) {
            std::filesystem::create_directories(physicalPath.parent_path(), ec);
        }

        int stride = width * channels;
        int result = stbi_write_png(physicalPath.string().c_str(), width, height, channels, data, stride);

        if (!result) {
            HELLIX_ERROR("ImageLoader: Falha ao salvar PNG em: {0}", physicalPath.string());
            return false;
        }

        return true;
    }

}