/*Console version for testing logic.  Should prolly be ignored and stuff because uhhh... */

#include "utils/logger.h"
#include "utils/system_utils.h"
#include "network/wifi_manager.h"
#include "network/proxy_manager.h"
#include "network/device_registry.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    LOG("UNESWA AutoConnect Core Test (32-bit)");
    LOG("OS: " + SystemUtils::get_system_summary());

    if (!SystemUtils::is_admin()) {
        LOG("Warning: No admin privs detected");
    }

    std::string sid, bday;
    std::cout << "Enter Student ID: ";
    std::getline(std::cin, sid);
    std::cout << "Enter Birthday (ddmmyyyy): ";
    std::getline(std::cin, bday);

    if (sid.empty() || bday.empty()) {
        LOG("Error: Empty creds");
        return 1;
    }

    LOG("Starting test...");

    ////TODO add proper connection flow here
    LOG("WiFi status: " + std::string(WiFiManager::is_connected() ? "Connected" : "Disconnected"));

    LOG("Test finished");
    return 0;
}
