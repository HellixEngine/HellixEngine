#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include "engine/core/buffer.hpp"

namespace hellix::core {

    /**
     * @brief Subsistema estático de Virtual File System (VFS) da HellixEngine.
     */
    class FileSystem {
    public:

        /*@todo concertar dependecia do Current Working Directory(cwd) adicionar formas de pegar o caminho do arquivo atual fisico e nao somente do local de execuçâo*/

        // =========================================================
        // INICIALIZAÇÃO E CICLO DE VIDA
        // =========================================================
        static void init();
        static void shutdown();

        // =========================================================
        // GERENCIAMENTO DE PONTOS DE MONTAGEM (VFS)
        // =========================================================
        /**
         * @brief Registra um alias de montagem virtual.
         * @param alias Prefixo com protocolo (ex: "engine://", "assets://").
         * @param physicalPath Diretório real no disco.
         */
        static void mount(std::string_view alias, const std::filesystem::path& physicalPath);

        /**
         * @brief Remove um ponto de montagem registrado.
         */
        static void unmount(std::string_view alias);

        /**
         * @brief Resolve um caminho virtual ou relativo para um caminho físico absoluto no SO.
         * @param virtualPath Caminho com prefixo virtual (ex: "assets://textures/wall.png").
         * @return Caminho absoluto resolvido no disco.
         */
        static std::filesystem::path resolve(std::string_view virtualPath);

        // =========================================================
        // LEITURA E ESCRITA
        // =========================================================
        /**
         * @brief Lê todo o conteúdo de um arquivo em formato de texto (string).
         */
        static std::string readText(std::string_view virtualPath);

        /**
         * @brief Lê todo o conteúdo de um arquivo em um buffer contíguo de bytes.
         */
        static Buffer readBytes(std::string_view virtualPath);

        /**
         * @brief Escreve uma string em um arquivo físico no disco.
         * Cria diretórios intermediários automaticamente se não existirem.
         */
        static bool writeText(std::string_view virtualPath, std::string_view content);

        /**
         * @brief Escreve um buffer de bytes brutos em um arquivo no disco.
         */
        static bool writeBytes(std::string_view virtualPath, const Buffer& buffer);

        // =========================================================
        // METADADOS E CONSULTAS
        // =========================================================
        static bool exists(std::string_view virtualPath);
        static bool isDirectory(std::string_view virtualPath);
        static uint64_t getFileSize(std::string_view virtualPath);
        static std::filesystem::file_time_type getLastModifiedTime(std::string_view virtualPath);

    private:
        static inline std::unordered_map<std::string, std::filesystem::path> s_mountPoints;
        static inline bool s_initialized = false;
    };

}
