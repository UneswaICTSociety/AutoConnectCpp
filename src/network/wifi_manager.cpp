#include "wifi_manager.h"
#include "../utils/logger.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

#if defined(_WIN32)
#include <windows.h>
#include <wlanapi.h>

#ifndef WLAN_SET_EAPHOST_DATA_ALL_USERS
#define WLAN_SET_EAPHOST_DATA_ALL_USERS 0x00000001
#endif

#endif

static const std::string WIFI_SSID = "uniswawifi-students";
static const std::string PASSWORD_PREFIX = "Uneswa";

std::string WiFiCredentials::normalize_birthday(std::string_view input) {
    std::string s(input);
    if (s.length() == 6) {
        std::string yy = s.substr(4, 2);
        return s.substr(0, 4) + "20" + yy;
    }
    return s;
}

std::string WiFiCredentials::get_password() const {
    if (!custom_password.empty()) {
        return custom_password;
    }
    return PASSWORD_PREFIX + normalize_birthday(birthday);
}

std::string WiFiManager::create_profile_xml(std::string_view ssid, std::string_view serverName, std::string_view certThumbprint) {
    std::string s(ssid);
    std::string sn(serverName);
    std::string ct(certThumbprint);
    if (ct.empty()) ct = "fd c8 c6 98 c5 4e b5 0b f9 fd aa ca c9 a5 84 ae 2d 60 b4 c3 ";
    return R"(<?xml version="1.0"?>
<WLANProfile xmlns="http://www.microsoft.com/networking/WLAN/profile/v1">
	<name>)" + s + R"(</name>
	<SSIDConfig>
		<SSID>
			<hex>756E69737761776966692D73747564656E7473</hex>
			<name>)" + s + R"(</name>
		</SSID>
	</SSIDConfig>
	<connectionType>ESS</connectionType>
	<connectionMode>auto</connectionMode>
	<MSM>
		<security>
			<authEncryption>
				<authentication>WPA2</authentication>
				<encryption>AES</encryption>
				<useOneX>true</useOneX>
			</authEncryption>
			<PMKCacheMode>enabled</PMKCacheMode>
			<PMKCacheTTL>720</PMKCacheTTL>
			<PMKCacheSize>128</PMKCacheSize>
			<preAuthMode>disabled</preAuthMode>
			<OneX xmlns="http://www.microsoft.com/networking/OneX/v1">
				<authMode>user</authMode>
				<EAPConfig><EapHostConfig xmlns="http://www.microsoft.com/provisioning/EapHostConfig"><EapMethod><Type xmlns="http://www.microsoft.com/provisioning/EapCommon">25</Type><VendorId xmlns="http://www.microsoft.com/provisioning/EapCommon">0</VendorId><VendorType xmlns="http://www.microsoft.com/provisioning/EapCommon">0</VendorType><AuthorId xmlns="http://www.microsoft.com/provisioning/EapCommon">0</AuthorId></EapMethod><Config xmlns="http://www.microsoft.com/provisioning/EapHostConfig"><Eap xmlns="http://www.microsoft.com/provisioning/BaseEapConnectionPropertiesV1"><Type>25</Type><EapType xmlns="http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV1"><ServerValidation><DisableUserPromptForServerValidation>false</DisableUserPromptForServerValidation><ServerNames>)" + sn + R"(</ServerNames><TrustedRootCA>)" + ct + R"(</TrustedRootCA></ServerValidation><FastReconnect>true</FastReconnect><InnerEapOptional>false</InnerEapOptional><Eap xmlns="http://www.microsoft.com/provisioning/BaseEapConnectionPropertiesV1"><Type>26</Type><EapType xmlns="http://www.microsoft.com/provisioning/MsChapV2ConnectionPropertiesV1"><UseWinLogonCredentials>false</UseWinLogonCredentials></EapType></Eap><EnableQuarantineChecks>false</EnableQuarantineChecks><RequireCryptoBinding>false</RequireCryptoBinding><PeapExtensions><PerformServerValidation xmlns="http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV2">true</PerformServerValidation><AcceptServerName xmlns="http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV2">true</AcceptServerName><PeapExtensionsV2 xmlns="http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV2"><AllowPromptingWhenServerCANotFound xmlns="http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV3">true</AllowPromptingWhenServerCANotFound></PeapExtensionsV2></PeapExtensions></EapType></Eap></Config></EapHostConfig></EAPConfig>
			</OneX>
		</security>
	</MSM>
	<MacRandomization xmlns="http://www.microsoft.com/networking/WLAN/profile/v3">
		<enableRandomization>false</enableRandomization>
		<randomizationSeed>4279424088</randomizationSeed>
	</MacRandomization>
</WLANProfile>)";
}

std::string WiFiManager::create_user_xml(std::string_view username, std::string_view password) {
    std::string u(username);
    std::string p(password);


    return R"(<?xml version="1.0"?>
<EapHostUserCredentials xmlns="http://www.microsoft.com/provisioning/EapHostUserCredentials"
xmlns:eapCommon="http://www.microsoft.com/provisioning/EapCommon"
xmlns:baseEap="http://www.microsoft.com/provisioning/BaseEapConnectionPropertiesV1">
<EapMethod>
<eapCommon:Type>25</eapCommon:Type>
</EapMethod>
<Credentials xmlns:eapUser="http://www.microsoft.com/provisioning/EapUserPropertiesV1"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:baseEap="http://www.microsoft.com/provisioning/BaseEapUserPropertiesV1"
xmlns:MsPeap="http://www.microsoft.com/provisioning/MsPeapUserPropertiesV1"
xmlns:MsChapV2="http://www.microsoft.com/provisioning/MsChapV2UserPropertiesV1">
<baseEap:Eap>
<baseEap:Type>25</baseEap:Type>
<MsPeap:EapType>
<MsPeap:RoutingIdentity>)" + u + R"(</MsPeap:RoutingIdentity>
<baseEap:Eap>
<baseEap:Type>26</baseEap:Type>
<MsChapV2:EapType>
<MsChapV2:Username>)" + u + R"(</MsChapV2:Username>
<MsChapV2:Password>)" + p + R"(</MsChapV2:Password>
<MsChapV2:LogonDomain></MsChapV2:LogonDomain>
</MsChapV2:EapType>
</baseEap:Eap>
</MsPeap:EapType>
</baseEap:Eap>
</Credentials>
</EapHostUserCredentials>)";
}

bool WiFiManager::set_eap_credentials(std::string_view ssid, std::string_view username, std::string_view password) {
#if defined(_WIN32)
    HANDLE h = NULL;
    DWORD v = 0;
    DWORD result = WlanOpenHandle(2, NULL, &v, &h);
    if (result != ERROR_SUCCESS) {
        LOG("Failed to open WLAN handle: " + std::to_string(result));
        return false;
    }

    PWLAN_INTERFACE_INFO_LIST l = NULL;
    result = WlanEnumInterfaces(h, NULL, &l);
    if (result != ERROR_SUCCESS) {
        LOG("Failed to enumerate WLAN interfaces: " + std::to_string(result));
        WlanCloseHandle(h, NULL);
        return false;
    }

    bool credentials_set = false;
    std::string xml = create_user_xml(username, password);
    std::wstring wxml(xml.begin(), xml.end());
    std::wstring wssid(ssid.begin(), ssid.end());

    for (DWORD i = 0; i < l->dwNumberOfItems; i++) {

        result = WlanSetProfileEapXmlUserData(
            h,
            &l->InterfaceInfo[i].InterfaceGuid,
            wssid.c_str(),
            WLAN_SET_EAPHOST_DATA_ALL_USERS,  // Use all users flag to match user=all profile
            wxml.c_str(),
            NULL
        );

        if (result == ERROR_SUCCESS) {
            LOG("Successfully set EAP credentials for interface " + std::to_string(i));
            credentials_set = true;
        } else {

            std::string error_msg;
            switch (result) {
                case ERROR_INVALID_PARAMETER:
                    error_msg = "Invalid parameter - check XML format";
                    break;
                case ERROR_ACCESS_DENIED:
                    error_msg = "Access denied - app needs administrator privileges";
                    break;
                case ERROR_NOT_FOUND:
                    error_msg = "Profile not found - may not have propagated yet";
                    break;
                case ERROR_INVALID_HANDLE:
                    error_msg = "Invalid WLAN handle";
                    break;
                case ERROR_NOT_SUPPORTED:
                    error_msg = "Operation not supported on this interface";
                    break;
                default:
                    error_msg = "Unknown error code: " + std::to_string(result);
                    break;
            }
            LOG("Failed to set EAP credentials for interface " + std::to_string(i) + ": " + error_msg);


            if (result == ERROR_ACCESS_DENIED) {
                LOG("Retrying without WLAN_SET_EAPHOST_DATA_ALL_USERS flag...");
                result = WlanSetProfileEapXmlUserData(
                    h,
                    &l->InterfaceInfo[i].InterfaceGuid,
                    wssid.c_str(),
                    0,  // No flags - current user only
                    wxml.c_str(),
                    NULL
                );

                if (result == ERROR_SUCCESS) {
                    LOG("Successfully set EAP credentials for interface " + std::to_string(i) + " (current user)");
                    credentials_set = true;
                } else {
                    LOG("Fallback also failed with error: " + std::to_string(result));
                }
            }
        }
    }

    WlanFreeMemory(l);
    WlanCloseHandle(h, NULL);

    if (!credentials_set) {
        LOG("CRITICAL: Failed to set EAP credentials on any interface");
        LOG("This will cause connection flickering - user may need to enter credentials manually");
    }

    return credentials_set;
#else
    return true;
#endif
}

WiFiResult WiFiManager::connect(const WiFiCredentials& creds) {
    if (SystemUtils::get_os_type() == "Windows") {
        return connect_win11_fixed(creds, creds.get_password());
    } else if (SystemUtils::get_os_type() == "Linux") {
        return connect_linux(creds, creds.get_password());
    }
    return { false, "OS not supported for WiFi" };
}

WiFiResult WiFiManager::connect_win11_fixed(const WiFiCredentials& creds, std::string_view password) {
    LOG("Starting WiFi connection...");

    SystemUtils::run_command("netsh wlan delete profile name=\"" + WIFI_SSID + "\"");

    std::string xml = create_profile_xml(WIFI_SSID, "", "");
    auto temp = std::filesystem::temp_directory_path() / (WIFI_SSID + "_profile.xml");

    LOG("Dumping WiFi profile XML..");
    {
        std::ofstream out(temp);
        out << xml;
    }

    LOG("Adding profile to netsh");
    auto res = SystemUtils::run_command("netsh wlan add profile filename=\"" + temp.string() + "\" user=all");
    std::filesystem::remove(temp);

    if (!res.success) {
        return { false, "Failed to add profile: " + res.stdout_output };
    }

    LOG("Profile added, waiting for propagation...");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    LOG("Setting EAP credentials...");
    if (!set_eap_credentials(WIFI_SSID, creds.student_id, password)) {
        LOG("ERROR: Failed to set EAP credentials via API - connection may require manual credential entry");
    } else {
        LOG("EAP credentials set successfully");
    }

    LOG("Connecting to " + WIFI_SSID);

    for (int i = 1; i <= 3; ++i) {
        LOG("Attempt " + std::to_string(i) + " of 3...");
        auto connect_res = SystemUtils::run_command("netsh wlan connect ssid=\"" + WIFI_SSID + "\" name=\"" + WIFI_SSID + "\"");

        if (connect_res.success) {
            LOG("In progress... waiting for DHCP etc.");
            for (int s = 0; s < 30; ++s) {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                if (is_connected()) {
                    LOG("Connected!");
                    return { true, "Connected to " + WIFI_SSID };
                }
                if (s % 5 == 0 && s > 0) LOG("Wait... (" + std::to_string(s * 2) + "s)");
            }
        }
        if (i < 3) {
            LOG("Failed, retrying...");
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    }

    return { false, "Couldn't connect. Check ID/Birthday and signal." };
}

WiFiResult WiFiManager::disconnect() {
    if (SystemUtils::get_os_type() == "Windows") {
        auto res = SystemUtils::run_command("netsh wlan disconnect");
        if (res.success) return { true, "Disconnected" };
        return { false, "Disconnect failed: " + res.stdout_output };
    } else {
        auto res = SystemUtils::run_command("nmcli connection down \"" + WIFI_SSID + "\"");
        if (res.success) return { true, "Disconnected" };
        return { false, "Disconnect failed: " + res.stdout_output };
    }
}

bool WiFiManager::is_connected() {
    if (SystemUtils::get_os_type() == "Windows") {
        auto res = SystemUtils::run_command("netsh wlan show interfaces");
        if (res.success) {
            std::string out = res.stdout_output;
            for (auto& c : out) c = std::tolower(c);
            bool connected = out.find("state") != std::string::npos && out.find("connected") != std::string::npos;
            std::string ssid = WIFI_SSID;
            for (auto& c : ssid) c = std::tolower(c);
            return connected && out.find(ssid) != std::string::npos;
        }
    } else {
        auto res = SystemUtils::run_command("nmcli -t -f NAME,TYPE,DEVICE connection show --active");
        if (res.success) {
            std::string out = res.stdout_output;
            for (auto& c : out) c = std::tolower(c);
            std::string ssid = WIFI_SSID;
            for (auto& c : ssid) c = std::tolower(c);
            return out.find("wifi") != std::string::npos && out.find(ssid) != std::string::npos;
        }
    }
    return false;
}

WiFiResult WiFiManager::remove_profile() {
    if (SystemUtils::get_os_type() == "Windows") {
        SystemUtils::run_command("netsh wlan delete profile name=\"" + WIFI_SSID + "\"");
        return { true, "Profile removed" };
    } else {
        if (remove_linux_connection(WIFI_SSID)) return { true, "Profile removed" };
        return { false, "Failed to remove profile" };
    }
}

WiFiResult WiFiManager::connect_linux(const WiFiCredentials& creds, std::string_view password) {
    remove_linux_connection(WIFI_SSID);
    std::vector<std::string> methods = {"peap", "ttls", "peap-md5"};
    std::string last_error;

    for (const auto& method : methods) {
        LOG("Trying " + method + "...");
        auto res = try_linux_method(method, creds, password);
        if (res.success) return res;
        last_error = res.message;
        remove_linux_connection(WIFI_SSID);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return { false, "All methods failed. Last error: " + last_error };
}

WiFiResult WiFiManager::try_linux_method(std::string_view method, const WiFiCredentials& creds, std::string_view password) {
    std::string nm_cmd;
    std::string pwd(password);
    if (method == "peap") {
        nm_cmd = "nmcli connection add type wifi con-name \"" + WIFI_SSID + "\" ifname \"*\" ssid \"" + WIFI_SSID + "\" "
                 "wifi-sec.key-mgmt wpa-eap 802-1x.eap peap 802-1x.phase2-auth mschapv2 "
                 "802-1x.identity \"" + creds.student_id + "\" 802-1x.password \"" + pwd + "\" "
                 "802-1x.system-ca-certs no 802-1x.password-flags 0 connection.autoconnect yes";
    } else if (method == "ttls") {
         nm_cmd = "nmcli connection add type wifi con-name \"" + WIFI_SSID + "\" ifname \"*\" ssid \"" + WIFI_SSID + "\" "
                 "wifi-sec.key-mgmt wpa-eap 802-1x.eap ttls 802-1x.phase2-auth mschapv2 "
                 "802-1x.identity \"" + creds.student_id + "\" 802-1x.anonymous-identity \"" + creds.student_id + "\" "
                 "802-1x.password \"" + pwd + "\" 802-1x.system-ca-certs no 802-1x.password-flags 0 connection.autoconnect yes";
    } else {
        nm_cmd = "nmcli connection add type wifi con-name \"" + WIFI_SSID + "\" ifname \"*\" ssid \"" + WIFI_SSID + "\" "
                 "wifi-sec.key-mgmt wpa-eap 802-1x.eap peap 802-1x.phase2-auth md5 "
                 "802-1x.identity \"" + creds.student_id + "\" 802-1x.password \"" + pwd + "\" "
                 "802-1x.system-ca-certs no 802-1x.password-flags 0 connection.autoconnect yes";
    }

    auto res = SystemUtils::run_command(nm_cmd);
    if (!res.success) return { false, "nmcli add failed: " + res.stdout_output };

    auto act_res = SystemUtils::run_command("nmcli connection up \"" + WIFI_SSID + "\"");
    if (act_res.success) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (is_connected()) return { true, "Connected" };
    }
    return { false, "Connection failed" };
}

bool WiFiManager::remove_linux_connection(std::string_view name) {
    return SystemUtils::run_command("nmcli connection delete \"" + std::string(name) + "\"").success;
}
