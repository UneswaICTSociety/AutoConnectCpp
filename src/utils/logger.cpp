#include "logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger() {
    if (!std::filesystem::exists("logs")) std::filesystem::create_directory("logs");
    log_file.open("logs/autoconnect.log", std::ios::app);
}

Logger::~Logger() {
    if (log_file.is_open()) log_file.close();
}

void Logger::log(std::string_view message) {
    std::lock_guard<std::mutex> lock(log_mutex);
    
    auto now = std::chrono::system_clock::now();
    auto time_val = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time_val), "%H:%M:%S") << "] " << message;
    
    std::string formatted = ss.str();
    
    if (log_file.is_open()) log_file << formatted << std::endl;
    std::cout << formatted << std::endl;
    memory_log.push_back(formatted);
    
    if (ui_callback) ui_callback(formatted);
}

std::string Logger::get_logs() const {
    std::string result;
    for (const auto& line : memory_log) result += line + "\n";
    return result;
}

void Logger::clear() {
    std::lock_guard<std::mutex> lock(log_mutex);
    memory_log.clear();
}

void Logger::set_callback(LogCallback callback) {
    std::lock_guard<std::mutex> lock(log_mutex);
    ui_callback = callback;
}

void LOG(std::string_view msg) {
    Logger::instance().log(msg);
}
