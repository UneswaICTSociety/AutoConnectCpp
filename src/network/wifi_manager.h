#pragma once

#include <string>
#include <vector>
#include <string_view>
#include "../utils/system_utils.h"

struct WiFiCredentials {
    std::string student_id;
    std::string birthday;
    std::string custom_password;

    std::string get_password() const;
    static std::string normalize_birthday(std::string_view input);
};

struct WiFiResult {
    bool success;
    std::string message;
};

class WiFiManager {
public:
    static WiFiResult connect(const WiFiCredentials& creds);
    static WiFiResult disconnect();
    static bool is_connected();
    static WiFiResult remove_profile();

private:
    static std::string create_profile_xml(std::string_view ssid, std::string_view serverName, std::string_view certThumbprint);
    static std::string create_user_xml(std::string_view username, std::string_view password);
    static bool set_eap_credentials(std::string_view ssid, std::string_view username, std::string_view password);
    static WiFiResult connect_win11_fixed(const WiFiCredentials& creds, std::string_view password);
    
    static WiFiResult connect_linux(const WiFiCredentials& creds, std::string_view password);
    static WiFiResult try_linux_method(std::string_view method, const WiFiCredentials& creds, std::string_view password);
    static bool remove_linux_connection(std::string_view name);
};
