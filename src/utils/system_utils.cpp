#include "system_utils.h"
#include <array>
#include <memory>
#include <iostream>
#include <sstream>

#if defined(_WIN32)
    #include <windows.h>
    #include <shlobj.h>
    #define POPEN _popen
    #define PCLOSE _pclose
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <pwd.h>
    #define POPEN popen
    #define PCLOSE pclose
#endif

namespace SystemUtils {

    CommandResult run_command(std::string_view cmd, int timeout_seconds) {
        CommandResult result;
        result.exit_code = -1;
        result.success = false;

        std::string full_cmd = std::string(cmd) + " 2>&1";
        
        std::array<char, 128> buffer;
        std::string output;
        
        FILE* pipe = POPEN(full_cmd.c_str(), "r");
        if (!pipe) {
            result.stderr_output = "popen() failed!";
            return result;
        }

        try {
            while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                output += buffer.data();
            }
        } catch (...) {
            result.stderr_output = "Exception reading output from pipe";
        }

        int return_code = PCLOSE(pipe);
        
        result.stdout_output = output;
        result.exit_code = return_code;
        result.success = (return_code == 0);
        
        return result;
    }

    bool is_admin() {
#if defined(_WIN32)
        BOOL fRet = FALSE;
        HANDLE hToken = NULL;
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
            TOKEN_ELEVATION elevation;
            DWORD cbSize = sizeof(TOKEN_ELEVATION);
            if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &cbSize)) {
                fRet = elevation.TokenIsElevated;
            }
        }
        if (hToken) {
            CloseHandle(hToken);
        }
        return fRet;
#else
        return geteuid() == 0;
#endif
    }

    std::string get_os_type() {
#if defined(_WIN32)
        return "Windows";
#elif defined(__APPLE__)
        return "Darwin";
#elif defined(__linux__)
        return "Linux";
#else
        return "Unknown";
#endif
    }
    
    std::string get_system_summary() {
        std::stringstream ss;
        ss << get_os_type();
#if defined(_WIN32)
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        
        #pragma warning(push)
        #pragma warning(disable: 4996) 
        if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
            ss << " " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
            ss << " Build " << osvi.dwBuildNumber;
        }
        #pragma warning(pop)
#endif
        return ss.str();
    }

}
