#pragma once

#include <string>
#include <string_view>
#include "../utils/system_utils.h"

struct ProxyResult {
    bool success;
    std::string message;
};

class ProxyManager {
public:
    static ProxyResult apply_settings();
    static ProxyResult enable_pac();
    static ProxyResult enable_manual_proxy();
    static ProxyResult disable_proxy();
    static bool is_configured();

private:
    static const std::string PROXY_HOST;
    static const int PROXY_PORT;
    static const std::string PAC_URL;

    static ProxyResult enable_linux_proxy();
    static ProxyResult disable_linux_proxy();
    static ProxyResult enable_pac_linux(const std::string& pac_url);
    static void configure_gsettings(bool enable);
    static void configure_kde(bool enable);
    static bool update_shell_file(std::string_view path, bool enable);
};
