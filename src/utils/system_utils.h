#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace SystemUtils {

    struct CommandResult {
        int exit_code;
        std::string stdout_output;
        std::string stderr_output;
        bool success;
    };

    CommandResult run_command(std::string_view cmd, int timeout_seconds = 30);

    bool is_admin();
    std::string get_os_type();
    std::string get_system_summary();

}
