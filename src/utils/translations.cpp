#include "translations.h"

Translations& Translations::instance() {
    static Translations instance;
    return instance;
}

Translations::Translations() {
    load_translations();
}

void Translations::set_language(Language lang) {
    current_language = lang;
}

Language Translations::get_language() const {
    return current_language;
}

std::string Translations::get(std::string_view key) const {
    std::string lang_key = (current_language == Language::SISWATI) ? "ss" : "en";
    std::string k(key);
    
    auto lang_it = translations.find(lang_key);
    if (lang_it != translations.end()) {
        auto key_it = lang_it->second.find(k);
        if (key_it != lang_it->second.end()) return key_it->second;
    }
    
    auto en_it = translations.find("en");
    if (en_it != translations.end()) {
        auto key_it = en_it->second.find(k);
        if (key_it != en_it->second.end()) return key_it->second;
    }
    
    return k;
}

std::string Translations::t(std::string_view key) {
    return instance().get(key);
}

void Translations::load_translations() {
    translations["en"] = {
        {"app_title", "UNESWA WiFi AutoConnect"},
        {"app_subtitle", "ICT Society - University of Eswatini"},
        {"app_version", "UNESWA WiFi AutoConnect v1.3.7 starting..."},
        {"status_fully_connected", "Status: Fully Connected"},
        {"status_partially_connected", "Status: Partially Connected"},
        {"status_not_connected", "Status: Not Connected"},
        {"wifi_connected", "WiFi: Connected"},
        {"wifi_disconnected", "WiFi: Disconnected"},
        {"proxy_configured", "Proxy: Configured"},
        {"proxy_not_configured", "Proxy: Not Configured"},
        {"student_credentials", "Student Credentials"},
        {"student_id", "Student ID:"},
        {"student_id_placeholder", "e.g., 20211234"},
        {"password_mode", "Password Mode:"},
        {"birthday_mode", "Birthday (Default)"},
        {"custom_password_mode", "Custom Password"},
        {"birthday", "Birthday:"},
        {"password", "Password:"},
        {"birthday_placeholder", "ddmmyyyy (e.g., 12052001)"},
        {"password_placeholder", "Your custom password"},
        {"birthday_format", "Format: ddmmyyyy (e.g., 12052001 for 12 May 2001)"},
        {"password_format", "Use this if you changed your default password"},
        {"complete_setup", "Complete Setup (Ctrl+Enter)"},
        {"complete_setup_working", "Working..."},
        {"individual_actions", "Individual Actions"},
        {"wifi_only", "WiFi Only"},
        {"proxy_only", "Config Network"},
        {"register_device", "Register Device"},
        {"test_connection", "Test Connection"},
        {"reset_uneswa", "Reset UNESWA"},
        {"activity_log", "Activity Log"},
        {"system_info", "System: "},
        {"admin_warning", "Warning: Not running with administrator privileges"},
        {"admin_warning_detail", "Some features may not work correctly"},
        {"operation_in_progress", "Operation already in progress"},
        {"credentials_required", "Student ID and birthday are required"},
        {"starting_setup", "Starting setup..."},
        {"setup_time_warning", "This may take 30-60 seconds..."},
        {"setup_completed_success", "Setup completed successfully!"},
        {"setup_completed_issues", "Setup finished with issues. Check logs."},
        {"testing_connection", "Testing connection..."},
        {"connection_all_operational", "Test: All operational"},
        {"connection_wifi_only", "Test: WiFi ok, proxy missing"},
        {"connection_not_connected", "Test: Not connected"},
        {"resetting_settings", "Resetting UNESWA settings..."},
        {"reset_complete", "Reset done"},
        {"wifi_success", "✓ WiFi: "},
        {"wifi_error", "✗ WiFi: "},
        {"registration_success", "✓ Registration: "},
        {"registration_error", "✗ Registration: "},
        {"proxy_success", "✓ Proxy: "},
        {"proxy_error", "✗ Proxy: "},
        {"language", "Language"},
        {"english", "English"},
        {"siswati", "siSwati"},
        {"info_title", "Password Information"},
        {"info_birthday_explanation", "Use if default password"},
        {"info_password_explanation", "Use if custom password"},
        {"info_tip", "Tip: Default is Birthday mode"}
    };
    
    translations["ss"] = {
        {"app_title", "UNESWA WiFi Kuxhumanisa"},
        {"app_subtitle", "ICT Society - Nyuvesi yase-Eswatini"},
        {"status_fully_connected", "Simo: Kuxhumene"},
        {"status_not_connected", "Simo: Akukaxhumani"},
        {"student_credentials", "Tintfo Temfundzi"},
        {"student_id", "Inombolo Yemfundzi:"},
        {"complete_setup", "Lungisa Konke"},
        {"individual_actions", "Tintfo Letehlukahlukene"},
        {"wifi_only", "WiFi Kuphela"},
        {"proxy_only", "Lungisa I-Network"},
        {"register_device", "Bhalisa Lidivayisi"},
        {"reset_uneswa", "Buyisela UNESWA"},
        {"language", "Lulwimi"},
        {"english", "SiNgisi"},
        {"siswati", "siSwati"}
    };
}