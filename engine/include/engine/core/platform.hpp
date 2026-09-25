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
        /** @name Localização e console/terminal */
        ///@{
        /** @brief Habilita o suporte a UTF-8 no terminal. */
        static void enableUTF8();
        /** @brief Define a localidade usada pelo sistema. */
        static void setLocale(Locale loc = Locale::Default);
        /** @brief Oculta o console da aplicação. */
        static void hideConsole();
        /** @brief Exibe o console da aplicação. */
        static void showConsole();
        /** @brief Alterna a visibilidade do console. */
        static void toggleConsole();
        /** @brief Informa se o console está visível. */
        [[nodiscard]] static bool isConsoleVisible();
        ///@}

        /** @name Área de transferência */
        ///@{
        /**
         * @brief Copia um texto puro para a área de transferência do sistema operacional.
         */
        static bool setClipboardText(std::string_view text);

        /**
         * @brief Lê o texto atualmente presente na área de transferência do sistema.
         */
        [[nodiscard]] static std::string getClipboardText();
        ///@}

        /** @name Diálogos e integração com o sistema operacional */
        ///@{
        /**
         * @brief Exibe uma caixa de mensagem nativa bloqueante.
         */
        static void showMessageBox(std::string_view title, std::string_view message, MessageBoxType type = MessageBoxType::Info);

        /**
         * @brief Abre uma URL no navegador padrão ou uma pasta no gerenciador de arquivos.
         */
        static void openURL(std::string_view urlOrPath);
        ///@}

        /** @name Metadados do sistema */
        ///@{
        /** @brief Retorna o nome da plataforma em execução. */
        [[nodiscard]] static constexpr std::string_view getPlatformName() noexcept {
        #if defined(_WIN32) || defined(_WIN64)
            return "Windows"; // Testado no Windows 11.
        #elif defined(__linux__)
            return "Linux"; // Testado no Ubuntu 22.04 executado no WSL2.
        #elif defined(__APPLE__)
            return "macOS"; // Ainda não testado.
        #else
            return "Unknown";
        #endif
        }
        ///@}
    };

}