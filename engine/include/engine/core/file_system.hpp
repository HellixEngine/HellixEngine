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

        /** @todo Corrigir a dependência do diretório de trabalho atual (CWD), permitindo obter o caminho físico do arquivo atual. */

        /** @name Inicialização e ciclo de vida */
        ///@{
        /** @brief Inicializa o sistema de arquivos virtual. */
        static void init();
        /** @brief Libera os recursos do sistema de arquivos virtual. */
        static void shutdown();
        ///@}

        /** @name Gerenciamento de pontos de montagem (VFS) */
        ///@{
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
        ///@}

        /** @name Leitura e escrita */
        ///@{
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
        ///@}

        /** @name Metadados e consultas */
        ///@{
        /** @brief Verifica se o caminho virtual existe. */
        static bool exists(std::string_view virtualPath);
        /** @brief Verifica se o caminho virtual aponta para um diretório. */
        static bool isDirectory(std::string_view virtualPath);
        /** @brief Retorna o tamanho do arquivo em bytes. */
        static uint64_t getFileSize(std::string_view virtualPath);
        /** @brief Retorna a data da última modificação do arquivo. */
        static std::filesystem::file_time_type getLastModifiedTime(std::string_view virtualPath);
        ///@}

    private:
        static inline std::unordered_map<std::string, std::filesystem::path> s_mountPoints;
        static inline bool s_initialized = false;
    };

}
