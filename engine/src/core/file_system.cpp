#include "engine/core/file_system.hpp"
#include "engine/core/log.hpp"

#include <fstream>

namespace hellix::core {

    void FileSystem::init() {
        if (s_initialized) return;

        s_mountPoints.clear();

        // Ponto de montagem base relativo ao diretório de execução atual
        std::filesystem::path currentPath = std::filesystem::current_path();

        mount("engine://", currentPath / "engine" / "assets");
        mount("assets://", currentPath / "assets");

        s_initialized = true;
        HELLIX_INFO("FileSystem (VFS) inicializado com sucesso.");
    }

    void FileSystem::shutdown() {
        s_mountPoints.clear();
        s_initialized = false;
        HELLIX_INFO("FileSystem encerrado.");
    }

    void FileSystem::mount(std::string_view alias, const std::filesystem::path& physicalPath) {
        std::string aliasStr(alias);
        // Garante que o alias termine com "://" para padronização
        if (!aliasStr.ends_with("://")) {
            aliasStr += "://";
        }

        s_mountPoints[aliasStr] = std::filesystem::weakly_canonical(physicalPath);
        HELLIX_TRACE("VFS Montado: {0} -> {1}", aliasStr, s_mountPoints[aliasStr].string());
    }

    void FileSystem::unmount(std::string_view alias) {
        std::string aliasStr(alias);
        if (!aliasStr.ends_with("://")) {
            aliasStr += "://";
        }
        s_mountPoints.erase(aliasStr);
    }
    std::filesystem::path FileSystem::resolve(std::string_view virtualPath) {
        // Auto-inicializa se alguém tentar usar o FileSystem antes da hora
        if (!s_initialized) {
            init();
        }

        std::string pathStr(virtualPath);

        for (const auto& [alias, physicalPath] : s_mountPoints) {
            // Verifica se o caminho começa com o alias (ex: "assets://")
            if (pathStr.starts_with(alias)) {
                std::string relativeSubPath = pathStr.substr(alias.length());

                // Remove barras iniciais redundantes se houver (ex: assets:///ficheiro.txt)
                while (!relativeSubPath.empty() && (relativeSubPath.front() == '/' || relativeSubPath.front() == '\\')) {
                    relativeSubPath.erase(0, 1);
                }

                return physicalPath / relativeSubPath;
            }
        }

        return std::filesystem::path(pathStr);
    }

    std::string FileSystem::readText(std::string_view virtualPath) {
        std::filesystem::path physicalPath = resolve(virtualPath);

        if (!std::filesystem::exists(physicalPath) || std::filesystem::is_directory(physicalPath)) {
            HELLIX_ERROR("FileSystem::readText: Arquivo inexistente ou invalido: {0}", physicalPath.string());
            return "";
        }

        std::ifstream file(physicalPath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            HELLIX_ERROR("FileSystem::readText: Falha ao abrir: {0}", physicalPath.string());
            return "";
        }

        const auto size = std::filesystem::file_size(physicalPath);
        std::string result(size, '\0');
        file.read(result.data(), static_cast<std::streamsize>(size));

        return result;
    }

    Buffer FileSystem::readBytes(std::string_view virtualPath) {
        std::filesystem::path physicalPath = resolve(virtualPath);

        if (!std::filesystem::exists(physicalPath) || std::filesystem::is_directory(physicalPath)) {
            HELLIX_ERROR("FileSystem::readBytes: Arquivo inexistente ou invalido: {0}", physicalPath.string());
            return Buffer{};
        }

        std::ifstream file(physicalPath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            HELLIX_ERROR("FileSystem::readBytes: Falha ao abrir: {0}", physicalPath.string());
            return Buffer{};
        }

        const uint64_t size = std::filesystem::file_size(physicalPath);
        Buffer buffer(size);
        file.read(buffer.as<char>(), static_cast<std::streamsize>(size));

        return buffer;
    }

    bool FileSystem::writeText(std::string_view virtualPath, std::string_view content) {
        std::filesystem::path physicalPath = resolve(virtualPath);

        std::error_code ec;
        if (physicalPath.has_parent_path()) {
            std::filesystem::create_directories(physicalPath.parent_path(), ec);
            if (ec) {
                HELLIX_ERROR("FileSystem::writeText: Falha ao criar pastas para '{0}': {1}",
                                  physicalPath.string(), ec.message());
                return false;
            }
        }

        std::ofstream file(physicalPath, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            HELLIX_ERROR("FileSystem::writeText: Falha ao abrir ficheiro '{0}' para gravacao.", physicalPath.string());
            return false;
        }

        file.write(content.data(), static_cast<std::streamsize>(content.size()));
        return true;
    }

    bool FileSystem::writeBytes(std::string_view virtualPath, const Buffer& buffer) {
        if (!buffer.isValid()) {
            HELLIX_WARN("FileSystem::writeBytes: Tentativa de gravar buffer vazio/invalido.");
            return false;
        }

        std::filesystem::path physicalPath = resolve(virtualPath);

        if (physicalPath.has_parent_path()) {
            std::filesystem::create_directories(physicalPath.parent_path());
        }

        std::ofstream file(physicalPath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            HELLIX_ERROR("FileSystem::writeBytes: Falha ao abrir para gravacao: {0}", physicalPath.string());
            return false;
        }

        file.write(buffer.as<const char>(), static_cast<std::streamsize>(buffer.size));
        return true;
    }

    bool FileSystem::exists(std::string_view virtualPath) {
        return std::filesystem::exists(resolve(virtualPath));
    }

    bool FileSystem::isDirectory(std::string_view virtualPath) {
        return std::filesystem::is_directory(resolve(virtualPath));
    }

    uint64_t FileSystem::getFileSize(std::string_view virtualPath) {
        std::filesystem::path path = resolve(virtualPath);
        if (!std::filesystem::exists(path) || std::filesystem::is_directory(path)) {
            return 0;
        }
        return std::filesystem::file_size(path);
    }

    std::filesystem::file_time_type FileSystem::getLastModifiedTime(std::string_view virtualPath) {
        std::filesystem::path path = resolve(virtualPath);
        if (!std::filesystem::exists(path)) {
            return std::filesystem::file_time_type{};
        }
        return std::filesystem::last_write_time(path);
    }

}