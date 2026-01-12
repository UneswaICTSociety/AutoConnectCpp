#pragma once

#include <string>
#include <string_view>

struct RegistrationResult {
    bool success;
    std::string message;
    bool already_registered;
};

class DeviceRegistry {
public:
    static RegistrationResult register_device(std::string_view student_id, std::string_view password);
    
private:
    static RegistrationResult try_registration_url(std::string_view url, 
                                                    std::string_view student_id,
                                                    std::string_view password);
};
