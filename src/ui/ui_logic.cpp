#include "ui_logic.h"
#include "../utils/logger.h"
#include "../utils/translations.h"
#include "../network/wifi_manager.h"
#include "../network/proxy_manager.h"
#include "../network/device_registry.h"
#include <thread>
#include <string_view> // const string& more or less.
#include <mutex>
#include <iostream>

UILogic::UILogic(AppWindow* window) : app_window(window), is_working(false) {
    Logger::instance().set_callback([this](std::string_view msg) {
        this->on_log_message(msg);
    });
    update_ui_language();
}

void UILogic::on_log_message(std::string_view msg) {
    std::string s(msg);

    //WIndows keeps crsahing so I think using a weak reference approach will stop that...
    // yeah, that works, lol.
    slint::invoke_from_event_loop([this, s]() {
        std::lock_guard<std::mutex> lock(ui_mutex);
        if (app_window) {
            try {
                std::string updated = std::string(app_window->get_log_text()) + s + "\n";
                app_window->set_log_text(slint::SharedString(updated));
            } catch (...) {
                std::cerr << "UI update error at around line 23 in ui_lohivc.cpp in ui folder\n";
                return;
            }
        }
    });
}

void UILogic::update_status() {
    std::lock_guard<std::mutex> lock(ui_mutex);
    if (!app_window) return;

    try {
        bool wifi_conn = WiFiManager::is_connected();
        bool proxy_conf = ProxyManager::is_configured();

        AppStatus status;
        status.wifi_connected = wifi_conn;
        status.proxy_configured = proxy_conf;

        if (wifi_conn && proxy_conf) status.overall_status = slint::SharedString(T("status_fully_connected"));
        else if (wifi_conn) status.overall_status = slint::SharedString(T("status_partially_connected"));
        else status.overall_status = slint::SharedString(T("status_not_connected"));

        app_window->set_status(status);
    } catch (...) {
        std::cerr << "UI update error at around line 48-ish in ui_lohivc.cpp in ui folder\n";
    }
}

void UILogic::update_ui_language() {
    std::lock_guard<std::mutex> lock(ui_mutex);
    if (!app_window) return;

    try {
        app_window->set_app_title(slint::SharedString(T("app_title")));
        app_window->set_app_subtitle(slint::SharedString(T("app_subtitle")));
        app_window->set_student_credentials_text(slint::SharedString(T("student_credentials")));
        app_window->set_student_id_text(slint::SharedString(T("student_id")));
        app_window->set_student_id_placeholder_text(slint::SharedString(T("student_id_placeholder")));
        app_window->set_password_mode_text(slint::SharedString(T("password_mode")));
        app_window->set_birthday_mode_text(slint::SharedString(T("birthday_mode")));
        app_window->set_custom_password_mode_text(slint::SharedString(T("custom_password_mode")));
        app_window->set_birthday_text(slint::SharedString(T("birthday")));
        app_window->set_password_text(slint::SharedString(T("password")));
        app_window->set_birthday_placeholder_text(slint::SharedString(T("birthday_placeholder")));
        app_window->set_password_placeholder_text(slint::SharedString(T("password_placeholder")));
        app_window->set_birthday_format_text(slint::SharedString(T("birthday_format")));
        app_window->set_password_format_text(slint::SharedString(T("password_format")));
        app_window->set_complete_setup_text(slint::SharedString(T("complete_setup")));
        app_window->set_complete_setup_working_text(slint::SharedString(T("complete_setup_working")));
        app_window->set_individual_actions_text(slint::SharedString(T("individual_actions")));
        app_window->set_wifi_only_text(slint::SharedString(T("wifi_only")));
        app_window->set_proxy_only_text(slint::SharedString(T("proxy_only")));
        app_window->set_register_device_text(slint::SharedString(T("register_device")));
        app_window->set_test_connection_text(slint::SharedString(T("test_connection")));
        app_window->set_reset_uneswa_text(slint::SharedString(T("reset_uneswa")));
        app_window->set_activity_log_text(slint::SharedString(T("activity_log")));
        app_window->set_language_text(slint::SharedString(T("language")));
        app_window->set_english_text(slint::SharedString(T("english")));
        app_window->set_siswati_text(slint::SharedString(T("siswati")));
        app_window->set_info_text(slint::SharedString("ℹ"));

        update_status();
    } catch (...) {
        std::cerr << "UI update error at around line 23 in ui_lohivc.cpp in ui folder\n";
        return;
    }
}

void UILogic::on_language_changed(bool is_siswati) {
    try {
        Translations::instance().set_language(is_siswati ? Language::SISWATI : Language::ENGLISH);
        update_ui_language();
        LOG(T("language") + ": " + (is_siswati ? T("siswati") : T("english")));
    } catch (...) {
        std::cerr << "UI language error at around line 103 in ui_lohivc.cpp in ui folder\n";
        return;
    }
}

void UILogic::on_info_clicked() {
    try {
        LOG("=== " + T("info_title") + " ===");
        LOG("📅 " + T("birthday_mode") + ": " + T("info_birthday_explanation"));
        LOG("🔑 " + T("custom_password_mode") + ": " + T("info_password_explanation"));
        LOG("💡 " + T("info_tip"));
        LOG("===============================");
    } catch (...) {
        std::cerr << "UI update error at around line 110 in ui_lohivc.cpp in ui folder\n";
        return;
    }
}

void UILogic::on_quit_app() {
    std::lock_guard<std::mutex> lock(ui_mutex);
    try {
        if (app_window) {
            app_window->hide();
        }
    } catch (...) {
        std::cerr << "UI update error at around line 110 in ui_lohivc.cpp in ui folder\n";
        return;
    }
}

void UILogic::complete_setup() {
    if (is_working) return;

    std::string sid, bday_or_pass;
    bool use_custom = false;

    {
        std::lock_guard<std::mutex> lock(ui_mutex);
        if (!app_window) return;

        try {
            sid = std::string(app_window->get_student_id());
            bday_or_pass = std::string(app_window->get_birthday());
            use_custom = app_window->get_use_custom_password();
        } catch (...) {
            return;
        }
    }

    if (sid.empty() || bday_or_pass.empty()) {
        LOG(T("credentials_required"));
        return;
    }

    is_working = true;
    set_working_state(true);

    // Logic object needs to stay alive during te thread execution
    auto self = shared_from_this();
    std::thread([self, sid, bday_or_pass, use_custom]() {
        try {
            LOG(T("starting_setup"));
            LOG(T("setup_time_warning"));

            WiFiCredentials creds;
            creds.student_id = sid;
            if (use_custom) creds.custom_password = bday_or_pass;
            else creds.birthday = bday_or_pass;

            WiFiResult wifi_res = WiFiManager::connect(creds);
            LOG(std::string(T(wifi_res.success ? "wifi_success" : "wifi_error")) + wifi_res.message);

            RegistrationResult reg_res = DeviceRegistry::register_device(sid, creds.get_password());
            LOG(std::string(T(reg_res.success ? "registration_success" : "registration_error")) + reg_res.message);

            ProxyResult proxy_res = ProxyManager::apply_settings();
            LOG(std::string(T(proxy_res.success ? "proxy_success" : "proxy_error")) + proxy_res.message);

            if (wifi_res.success && proxy_res.success) LOG("\n" + T("setup_completed_success"));
            else LOG("\n" + T("setup_completed_issues"));

            self->update_status();
        } catch (...) {
            LOG("Error during setup");
        }

        self->is_working = false;
        self->set_working_state(false);
    }).detach();
}

void UILogic::wifi_only() {
    if (is_working) return;

    std::string sid, bday_or_pass;
    bool use_custom = false;

    {
        std::lock_guard<std::mutex> lock(ui_mutex);
        if (!app_window) return;

        try {
            sid = std::string(app_window->get_student_id());
            bday_or_pass = std::string(app_window->get_birthday());
            use_custom = app_window->get_use_custom_password();
        } catch (...) {
            return;
        }
    }

    if (sid.empty() || bday_or_pass.empty()) {
        LOG(T("credentials_required"));
        return;
    }

    is_working = true;
    set_working_state(true);

    auto self = shared_from_this();
    std::thread([self, sid, bday_or_pass, use_custom]() {
        try {
            WiFiCredentials creds;
            creds.student_id = sid;
            if (use_custom) creds.custom_password = bday_or_pass;
            else creds.birthday = bday_or_pass;
            LOG(WiFiManager::connect(creds).message);
            self->update_status();
        } catch (...) {
            LOG("Error during WiFi setup");
        }

        self->is_working = false;
        self->set_working_state(false);
    }).detach();
}

void UILogic::proxy_only() {
    if (is_working) return;

    is_working = true;
    set_working_state(true);

    auto self = shared_from_this();
    std::thread([self]() {
        try {
            LOG(ProxyManager::apply_settings().message);
            self->update_status();
        } catch (...) {
            LOG("Error during proxy setup");
        }

        self->is_working = false;
        self->set_working_state(false);
    }).detach();
}

void UILogic::register_device() {
    if (is_working) return;

    std::string sid, bday_or_pass;
    bool use_custom = false;

    {
        std::lock_guard<std::mutex> lock(ui_mutex);
        if (!app_window) return;

        try {
            sid = std::string(app_window->get_student_id());
            bday_or_pass = std::string(app_window->get_birthday());
            use_custom = app_window->get_use_custom_password();
        } catch (...) {
            return;
        }
    }

    if (sid.empty() || bday_or_pass.empty()) return;

    is_working = true;
    set_working_state(true);

    auto self = shared_from_this();
    std::thread([self, sid, bday_or_pass, use_custom]() {
        try {
            WiFiCredentials creds;
            creds.student_id = sid;
            if (use_custom) creds.custom_password = bday_or_pass;
            else creds.birthday = bday_or_pass;
            LOG(DeviceRegistry::register_device(sid, creds.get_password()).message);
            self->update_status();
        } catch (...) {
            LOG("Error during device registration");
        }

        self->is_working = false;
        self->set_working_state(false);
    }).detach();
}

void UILogic::test_connection() {
    if (is_working) return;

    is_working = true;
    set_working_state(true);

    auto self = shared_from_this();
    std::thread([self]() {
        try {
            LOG(T("testing_connection"));
            bool wifi = WiFiManager::is_connected();
            bool proxy = ProxyManager::is_configured();
            if (wifi && proxy) LOG(T("connection_all_operational"));
            else if (wifi) LOG(T("connection_wifi_only"));
            else LOG(T("connection_not_connected"));
            self->update_status();
        } catch (...) {
            LOG("Error during connection test");
        }

        self->is_working = false;
        self->set_working_state(false);
    }).detach();
}

void UILogic::reset_all() {
    if (is_working) return;

    is_working = true;
    set_working_state(true);

    auto self = shared_from_this();
    std::thread([self]() {
        try {
            LOG(T("resetting_settings"));
            LOG(WiFiManager::remove_profile().message);
            LOG(ProxyManager::disable_proxy().message);
            LOG(T("reset_complete"));
            self->update_status();
        } catch (...) {
            LOG("Error during reset");
        }

        self->is_working = false;
        self->set_working_state(false);
    }).detach();
}

void UILogic::set_working_state(bool working) {
    slint::invoke_from_event_loop([this, working]() {
        std::lock_guard<std::mutex> lock(ui_mutex);
        if (app_window) {
            try {
                app_window->set_is_working(working);
            } catch (...) {
                std::cerr << "UI working state error at around line 351 in ui_lohivc.cpp in ui folder\n";
                return;
            }
        }
    });
}
