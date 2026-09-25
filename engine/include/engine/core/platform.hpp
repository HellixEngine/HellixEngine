#pragma once

#include <string>
#include <string_view>

namespace hellix::core {

    enum class Locale {
        Default,
        PtBR,
        EnUS
    };

    enum class MessageBoxType {
        Info,
        Warning,
        Error
    };

    /**
     * @brief Subsistema estático multiplataforma para controle do SO, terminal e janelas nativas.
     */
    class Platform {
    public:
        // =========================================================
        // LOCALIZAÇÃO E CONSOLE/TERMINAL
        // =========================================================
        static void enableUTF8();
        static void setLocale(Locale loc = Locale::Default);
        static void hideConsole();
        static void showConsole();
        static void toggleConsole();
        [[nodiscard]] static bool isConsoleVisible();

        // =========================================================
        // ÁREA DE TRANSFERÊNCIA (CLIPBOARD)
        // =========================================================
        /**
         * @brief Copia um texto puro para o clipboard do sistema operacional.
         */
        static bool setClipboardText(std::string_view text);

        /**
         * @brief Lê o texto atualmente presente no clipboard do sistema.
         */
        [[nodiscard]] static std::string getClipboardText();

        // =========================================================
        // DIÁLOGOS E INTEGRAÇÃO COM O SISTEMA OPERATIVO
        // =========================================================
        /**
         * @brief Exibe uma caixa de mensagem nativa bloqueante.
         */
        static void showMessageBox(std::string_view title, std::string_view message, MessageBoxType type = MessageBoxType::Info);

        /**
         * @brief Abre um URL no navegador padrão ou uma pasta no gestor de ficheiros.
         */
        static void openURL(std::string_view urlOrPath);

        // =========================================================
        // METADADOS DE SISTEMA
        // =========================================================
        [[nodiscard]] static constexpr std::string_view getPlatformName() noexcept {
        #if defined(_WIN32) || defined(_WIN64)
            return "Windows"; // tested on 11
        #elif defined(__linux__)
            return "Linux"; //tested on Ubuntu 22.04 which is running on WSL2
        #elif defined(__APPLE__)
            return "macOS";//not tested
        #else
            return "Unknown";
        #endif
        }
    };

}