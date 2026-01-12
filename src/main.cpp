#include "ui/ui_logic.h"
#include "utils/logger.h"
#include "utils/system_utils.h"
#include "utils/translations.h"
#include <iostream>
#include <exception>
#include <memory>
#include <csignal>
#include <thread>
#include <chrono>

volatile std::sig_atomic_t shutdown_requested = 0;
void signal_handler(int sig) {
    shutdown_requested = 1;
}

int main(int argc, char** argv) {
    try {
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        LOG(T("app_version"));
        LOG(T("system_info") + SystemUtils::get_system_summary());

        if (!SystemUtils::is_admin()) {
            LOG(T("admin_warning"));
            LOG(T("admin_warning_detail"));
        }

        auto app = AppWindow::create();
        auto logic = std::make_shared<UILogic>(&*app);

        // Same thing as the other file. Avoid crashing if UILogic is destroyed
        std::weak_ptr<UILogic> weak_logic = logic;

        app->on_complete_setup([weak_logic]() {
            if (auto l = weak_logic.lock()) l->complete_setup();
        });
        app->on_wifi_only([weak_logic]() {
            if (auto l = weak_logic.lock()) l->wifi_only();
        });
        app->on_proxy_only([weak_logic]() {
            if (auto l = weak_logic.lock()) l->proxy_only();
        });
        app->on_register_device([weak_logic]() {
            if (auto l = weak_logic.lock()) l->register_device();
        });
        app->on_test_connection([weak_logic]() {
            if (auto l = weak_logic.lock()) l->test_connection();
        });
        app->on_reset_all([weak_logic]() {
            if (auto l = weak_logic.lock()) l->reset_all();
        });
        app->on_language_changed([weak_logic](bool ss) {
            if (auto l = weak_logic.lock()) l->on_language_changed(ss);
        });
        app->on_info_clicked([weak_logic]() {
            if (auto l = weak_logic.lock()) l->on_info_clicked();
        });
        app->on_quit_app([weak_logic]() {
            if (auto l = weak_logic.lock()) l->on_quit_app();
        });

        logic->update_status();
        app->run();

        // Clear the logic reference before cleanup
        logic.reset();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        LOG("App closed.");
        return 0;

    } catch (const std::exception& e) {
        LOG("Fatal crash: " + std::string(e.what()));
        return 1;
    } catch (...) {
        LOG("Unknown fatal error. Something went south.");
        return 1;
    }
}
