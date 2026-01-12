#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <iostream>
#include <vector>
#include <functional>
#include <string_view>

class Logger {
public:
    static Logger& instance();

    void log(std::string_view message);
    std::string get_logs() const;
    void clear();

    using LogCallback = std::function<void(std::string_view)>;
    void set_callback(LogCallback callback);

private:
    Logger();
    ~Logger();

    std::ofstream log_file;
    std::mutex log_mutex;
    std::vector<std::string> memory_log;
    LogCallback ui_callback = nullptr;
};

void LOG(std::string_view msg);
