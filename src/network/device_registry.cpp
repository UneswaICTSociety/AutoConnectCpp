#include "device_registry.h"
#include <cpr/cpr.h>
#include <iostream>

RegistrationResult DeviceRegistry::try_registration_url(
    std::string_view url,
    std::string_view sid,
    std::string_view pwd) {

    auto res = cpr::Post( // I mean, to be honest, this is rather self explanator. In pythgon we go requests_object.post("some stff here")
        cpr::Url{std::string(url)},
        cpr::Payload{
            {"user", std::string(sid)},
            {"pass", std::string(pwd)},
            {"submit", "ACCEPT"}
        },
        cpr::Timeout{10000}
    );

    if (res.status_code == 200 || res.status_code == 302) {
        //TODO: test this to make sure it works to the same degree as py
        std::string body = res.text;
        for (auto& c : body) c = std::tolower(c);

        if (body.find("not required") != std::string::npos ||
            body.find("not on a network que requires registration") != std::string::npos) {
            return { true, "Already registered (or not on campus)", true };
        }

        if (body.find("already registered") != std::string::npos ||
            body.find("hardware already registered") != std::string::npos) {
            return { true, "Device already registered", true };
        }

        if (body.find("success") != std::string::npos ||
            body.find("registered") != std::string::npos) {
            return { true, "Successfully registered!", false };
        }

        return { true, "Request sent. Restart if needed.", false };
    } else {
        return { false, "Portal error: " + std::to_string(res.status_code), false };
    }
}

RegistrationResult DeviceRegistry::register_device(
    std::string_view sid,
    std::string_view pwd) {

    std::string url = "http://netreg.uniswa.sz/cgi-bin/register.cgi";
    auto res = try_registration_url(url, sid, pwd);

    if (!res.success) {
        return { false, "Couldn't reach netreg. Check WiFi.", false }; // you can't register when you aren't connect to wifi at school
    }//I am serious

    return res;
}
