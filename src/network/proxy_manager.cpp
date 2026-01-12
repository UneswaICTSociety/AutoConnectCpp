#include "proxy_manager.h"
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cstdlib>

#if defined(_WIN32)
#include <windows.h>
#include <wininet.h>
#endif

const std::string ProxyManager::PROXY_HOST = "proxy02.uniswa.sz";
const int ProxyManager::PROXY_PORT = 3128;
const std::string ProxyManager::PAC_URL = "http://proxy02.uniswa.sz:3128/proxy.pac";

ProxyResult ProxyManager::apply_settings() {
    return enable_pac();
}

ProxyResult ProxyManager::enable_manual_proxy() {
#if defined(_WIN32)
    HKEY key;
    auto res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_WRITE, &key);

    if (res != ERROR_SUCCESS) return { false, "Failed to open registry" };

    std::string proxy_server = PROXY_HOST + ":" + std::to_string(PROXY_PORT);

    RegSetValueExA(key, "ProxyServer", 0, REG_SZ, (const BYTE*)proxy_server.c_str(), (DWORD)proxy_server.length() + 1);
    DWORD enable = 1;
    RegSetValueExA(key, "ProxyEnable", 0, REG_DWORD, (const BYTE*)&enable, sizeof(DWORD));
    RegDeleteValueA(key, "AutoConfigURL");
    DWORD auto_detect = 0;
    RegSetValueExA(key, "AutoDetect", 0, REG_DWORD, (const BYTE*)&auto_detect, sizeof(DWORD));
    RegCloseKey(key);

    InternetSetOptionA(NULL, 37, NULL, 0);
    InternetSetOptionA(NULL, 39, NULL, 0);
    SystemUtils::run_command("netsh winhttp set proxy " + proxy_server, 10);

    return { true, "Proxy active: " + proxy_server };
#else
    return enable_linux_proxy();
#endif
}

ProxyResult ProxyManager::enable_pac() {
    const std::string& pac_url = PAC_URL;
#if defined(_WIN32)
    HKEY key;
    auto res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_WRITE, &key);
    if (res != ERROR_SUCCESS) return { false, "Failed to open registry" };

    DWORD enable = 1;
    RegSetValueExA(key, "ProxyEnable", 0, REG_DWORD, (const BYTE*)&enable, sizeof(DWORD));
    RegSetValueExA(key, "AutoConfigURL", 0, REG_SZ, (const BYTE*)pac_url.c_str(), pac_url.size() + 1);
    RegCloseKey(key);

    InternetSetOptionA(NULL, 37, NULL, 0);
    InternetSetOptionA(NULL, 39, NULL, 0);

    return { true, "Proxy active: " + PAC_URL };
#else
    return enable_pac_linux(pac_url);
#endif
}

ProxyResult ProxyManager::enable_pac_linux(const std::string& pac_url) {
    SystemUtils::run_command("gsettings set org.gnome.system.proxy mode 'auto'");
    SystemUtils::run_command("gsettings set org.gnome.system.proxy autoconfig-url '" + pac_url + "'");
    
    // KDE
    SystemUtils::run_command("kwriteconfig5 --file kioslaverc --group 'Proxy Settings' --key ProxyType 2");
    SystemUtils::run_command("kwriteconfig5 --file kioslaverc --group 'Proxy Settings' --key proxyConfigScript '" + pac_url + "'");

    return { true, "Linux PAC enabled" };
}

ProxyResult ProxyManager::disable_proxy() {
#if defined(_WIN32)
    HKEY key;
    auto res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_WRITE, &key);
    if (res != ERROR_SUCCESS) return { false, "Failed to open registry" };

    DWORD enable = 0;
    RegSetValueExA(key, "ProxyEnable", 0, REG_DWORD, (const BYTE*)&enable, sizeof(DWORD));
    RegDeleteValueA(key, "ProxyServer");
    RegDeleteValueA(key, "AutoConfigURL");
    RegCloseKey(key);

    InternetSetOptionA(NULL, 37, NULL, 0);
    InternetSetOptionA(NULL, 39, NULL, 0);
    SystemUtils::run_command("netsh winhttp reset proxy", 10);

    return { true, "Proxy disabled" };
#else
    return disable_linux_proxy();
#endif
}

bool ProxyManager::is_configured() {
#if defined(_WIN32)
    HKEY key;
    auto res = RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_READ, &key);
    if (res != ERROR_SUCCESS) return false;

    DWORD enabled = 0, size = sizeof(DWORD);
    RegQueryValueExA(key, "ProxyEnable", NULL, NULL, (LPBYTE)&enabled, &size);
    char proxy_server[256] = {0};
    size = sizeof(proxy_server);
    RegQueryValueExA(key, "ProxyServer", NULL, NULL, (LPBYTE)proxy_server, &size);
    RegCloseKey(key);

    std::string s(proxy_server);
    return enabled && (s.find(PROXY_HOST) != std::string::npos);
#else
    char* proxy = std::getenv("http_proxy");
    return proxy && std::string(proxy).find(PROXY_HOST) != std::string::npos;
#endif
}

ProxyResult ProxyManager::enable_linux_proxy() {
    std::string url = "http://" + PROXY_HOST + ":" + std::to_string(PROXY_PORT);
#if !defined(_WIN32)
    setenv("http_proxy", url.c_str(), 1);
    setenv("https_proxy", url.c_str(), 1);
#endif
    char* home = std::getenv("HOME");
    if (home) {
        std::string h(home);
        update_shell_file(h + "/.bashrc", true);
        update_shell_file(h + "/.zshrc", true);
    }
    configure_gsettings(true);
    configure_kde(true);
    return { true, "Linux proxy enabled" };
}

ProxyResult ProxyManager::disable_linux_proxy() {
#if !defined(_WIN32)
    unsetenv("http_proxy");
    unsetenv("https_proxy");
#endif
    char* home = std::getenv("HOME");
    if (home) {
        std::string h(home);
        update_shell_file(h + "/.bashrc", false);
        update_shell_file(h + "/.zshrc", false);
    }
    configure_gsettings(false);
    configure_kde(false);
    return { true, "Linux proxy disabled" };
}

void ProxyManager::configure_gsettings(bool enable) {
    if (enable) {
        SystemUtils::run_command("gsettings set org.gnome.system.proxy mode 'manual'");
        SystemUtils::run_command("gsettings set org.gnome.system.proxy.http host '" + PROXY_HOST + "'");
        SystemUtils::run_command("gsettings set org.gnome.system.proxy.http port " + std::to_string(PROXY_PORT));
    } else {
        SystemUtils::run_command("gsettings set org.gnome.system.proxy mode 'none'");
    }
}

void ProxyManager::configure_kde(bool enable) {
    if (enable) {
        std::string url = "http://" + PROXY_HOST + ":" + std::to_string(PROXY_PORT);
        SystemUtils::run_command("kwriteconfig5 --file kioslaverc --group 'Proxy Settings' --key ProxyType 1");
        SystemUtils::run_command("kwriteconfig5 --file kioslaverc --group 'Proxy Settings' --key httpProxy '" + url + "'");
    } else {
        SystemUtils::run_command("kwriteconfig5 --file kioslaverc --group 'Proxy Settings' --key ProxyType 0");
    }
}

bool ProxyManager::update_shell_file(std::string_view path, bool enable) {
    std::string p(path);
    if (!std::filesystem::exists(p)) return false;

    std::ifstream in(p);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(in, line)) {
        if (line.find("# UNESWA WiFi AutoConnect") != std::string::npos) {
            while (std::getline(in, line)) if (line.find("# End UNESWA proxy") != std::string::npos) break;
        } else lines.push_back(line);
    }
    in.close();

    if (enable) {
        lines.push_back("");
        lines.push_back("# UNESWA WiFi AutoConnect proxy settings");
        std::string url = "http://" + PROXY_HOST + ":" + std::to_string(PROXY_PORT);
        lines.push_back("export http_proxy=\"" + url + "\"");
        lines.push_back("export https_proxy=\"" + url + "\"");
        lines.push_back("# End UNESWA proxy settings");
    }

    std::ofstream out(p);
    for (const auto& l : lines) out << l << "\n";
    return true;
}
