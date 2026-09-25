#include "engine/core/platform.hpp"
#include "engine/core/log.hpp"

#include <clocale>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <shellapi.h>
#elif defined(__linux__)
    #include <unistd.h>
    #include <array>
    #include <memory>
#endif

namespace hellix::core {

    // =========================================================
    // CONSOLE E LOCALIZAÇÃO
    // =========================================================

    void Platform::enableUTF8() {
    #if defined(_WIN32) || defined(_WIN64)
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif
        std::setlocale(LC_ALL, ".UTF-8");
    }

    void Platform::setLocale(Locale loc) {
        switch (loc) {
            case Locale::PtBR:
            #if defined(_WIN32) || defined(_WIN64)
                std::setlocale(LC_ALL, "Portuguese_Brazil.1252");
            #else
                std::setlocale(LC_ALL, "pt_BR.UTF-8");
            #endif
                break;

            case Locale::EnUS:
            #if defined(_WIN32) || defined(_WIN64)
                std::setlocale(LC_ALL, "English_United States.1252");
            #else
                std::setlocale(LC_ALL, "en_US.UTF-8");
            #endif
                break;

            case Locale::Default:
            default:
                std::setlocale(LC_ALL, "");
                break;
        }
    }

    void Platform::hideConsole() {
    #if defined(_WIN32) || defined(_WIN64)
        HWND consoleWindow = GetConsoleWindow();
        if (consoleWindow) ShowWindow(consoleWindow, SW_HIDE);
    #endif
    }

    void Platform::showConsole() {
    #if defined(_WIN32) || defined(_WIN64)
        HWND consoleWindow = GetConsoleWindow();
        if (consoleWindow) ShowWindow(consoleWindow, SW_SHOW);
    #endif
    }

    void Platform::toggleConsole() {
    #if defined(_WIN32) || defined(_WIN64)
        HWND consoleWindow = GetConsoleWindow();
        if (consoleWindow) {
            ShowWindow(consoleWindow, IsWindowVisible(consoleWindow) ? SW_HIDE : SW_SHOW);
        }
    #endif
    }

    bool Platform::isConsoleVisible() {
    #if defined(_WIN32) || defined(_WIN64)
        HWND consoleWindow = GetConsoleWindow();
        return consoleWindow && IsWindowVisible(consoleWindow);
    #else
        return true;
    #endif
    }

    // =========================================================
    // ÁREA DE TRANSFERÊNCIA (CLIPBOARD)
    // =========================================================

    bool Platform::setClipboardText(std::string_view text) {
    #if defined(_WIN32) || defined(_WIN64)
        if (!OpenClipboard(nullptr)) return false;
        EmptyClipboard();

        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (!hGlob) {
            CloseClipboard();
            return false;
        }

        memcpy(GlobalLock(hGlob), text.data(), text.size());
        static_cast<char*>(GlobalLock(hGlob))[text.size()] = '\0';
        GlobalUnlock(hGlob);

        SetClipboardData(CF_TEXT, hGlob);
        CloseClipboard();
        return true;
    #elif defined(__linux__)
        // Fallback rápido usando xclip ou wl-copy se presentes no ambiente do utilizador
        std::string cmd = "wl-copy 2>/dev/null || xclip -selection clipboard 2>/dev/null";
        FILE* pipe = popen(cmd.c_str(), "w");
        if (!pipe) return false;
        fwrite(text.data(), sizeof(char), text.size(), pipe);
        pclose(pipe);
        return true;
    #else
        return false;
    #endif
    }

    std::string Platform::getClipboardText() {
    #if defined(_WIN32) || defined(_WIN64)
        if (!OpenClipboard(nullptr)) return "";
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (!hData) {
            CloseClipboard();
            return "";
        }

        char* pszText = static_cast<char*>(GlobalLock(hData));
        std::string text = pszText ? pszText : "";
        GlobalUnlock(hData);
        CloseClipboard();
        return text;
    #elif defined(__linux__)
        std::string cmd = "wl-paste 2>/dev/null || xclip -selection clipboard -o 2>/dev/null";
        std::array<char, 256> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) return "";
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    #else
        return "";
    #endif
    }

    // =========================================================
    // DIÁLOGOS E SHELL
    // =========================================================

    #if defined(_WIN32) || defined(_WIN64)
    #include <vector>

    // Função utilitária interna para converter UTF-8 para UTF-16 (wchar_t)
    static std::wstring utf8ToWide(std::string_view utf8Str) {
        if (utf8Str.empty()) return L"";

        int sizeNeeded = MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8Str.data(),
            static_cast<int>(utf8Str.size()),
            nullptr,
            0
        );

        if (sizeNeeded <= 0) return L"";

        std::wstring wideStr(sizeNeeded, 0);
        MultiByteToWideChar(
            CP_UTF8,
            0,
            utf8Str.data(),
            static_cast<int>(utf8Str.size()),
            &wideStr[0],
            sizeNeeded
        );

        return wideStr;
    }
#endif

void Platform::showMessageBox(std::string_view title, std::string_view message, MessageBoxType type) {
#if defined(_WIN32) || defined(_WIN64)
    UINT flags = MB_OK;
    switch (type) {
        case MessageBoxType::Info:    flags |= MB_ICONINFORMATION; break;
        case MessageBoxType::Warning: flags |= MB_ICONWARNING;     break;
        case MessageBoxType::Error:   flags |= MB_ICONERROR;       break;
    }

    // Converte de UTF-8 para UTF-16
    std::wstring wideTitle   = utf8ToWide(title);
    std::wstring wideMessage = utf8ToWide(message);

    // Usa a versão Wide (Unicode/UTF-16) da Win32 API
    MessageBoxW(nullptr, wideMessage.c_str(), wideTitle.c_str(), flags);

#elif defined(__linux__)
    std::string iconType = "--info";
    if (type == MessageBoxType::Warning) iconType = "--warning";
    else if (type == MessageBoxType::Error) iconType = "--error";

    // O Linux já trabalha nativamente com strings UTF-8 em bash/zenity/kdialog
    std::string cmd = "zenity " + iconType + " --title=\"" + std::string(title) + "\" --text=\"" + std::string(message) + "\" 2>/dev/null"
                      " || kdialog " + iconType + " \"" + std::string(message) + "\" --title \"" + std::string(title) + "\" 2>/dev/null";
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        HELLIX_WARN("[MessageBox Fallback] {0}: {1}", title, message);
    }
#endif
}

    void Platform::openURL(std::string_view urlOrPath) {
    #if defined(_WIN32) || defined(_WIN64)
        ShellExecuteA(nullptr, "open", urlOrPath.data(), nullptr, nullptr, SW_SHOWNORMAL);
    #elif defined(__linux__)
        std::string cmd = "xdg-open \"" + std::string(urlOrPath) + "\" &";
        std::system(cmd.c_str());
    #endif
    }

}