#pragma once

#include "app_window.h"
#include <string>
#include <string_view>
#include <memory>
#include <mutex>

class UILogic : public std::enable_shared_from_this<UILogic> {
public:
    UILogic(AppWindow* window);
    
    void complete_setup();
    void wifi_only();
    void proxy_only();
    void register_device();
    void test_connection();
    void reset_all();
    
    void on_language_changed(bool is_siswati);
    void update_ui_language();
    void on_info_clicked();
    void on_quit_app();
    
    void update_status();
    
private:
    AppWindow* app_window;
    bool is_working;
    std::mutex ui_mutex;
    
    void on_log_message(std::string_view msg);
    void set_working_state(bool working);
};
