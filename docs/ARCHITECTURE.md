# UNESWA WiFi AutoConnect - Architecture Overview

This document provides a comprehensive overview of the technical architecture, design decisions, and implementation details of the UNESWA WiFi AutoConnect application.

## High-Level Architecture

The application follows a layered architecture pattern with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                    User Interface Layer                     │
│                     (Slint UI)                             │
├─────────────────────────────────────────────────────────────┤
│                  Application Logic Layer                    │
│                    (UI Logic)                              │
├─────────────────────────────────────────────────────────────┤
│                   Business Logic Layer                      │
│              (Network, Device, Proxy Managers)             │
├─────────────────────────────────────────────────────────────┤
│                    Utility Layer                           │
│            (Logger, System Utils, Translations)            │
├─────────────────────────────────────────────────────────────┤
│                   Platform Layer                           │
│              (Windows APIs, System Calls)                  │
└─────────────────────────────────────────────────────────────┘
```

## Project Structure

```
AutoConnectCpp/
├── src/                           # Source code
│   ├── main.cpp                   # Application entry point (GUI)
│   ├── main_console.cpp           # Console version for testing
│   ├── network/                   # Network management
│   │   ├── wifi_manager.cpp/.h    # WiFi operations
│   │   ├── proxy_manager.cpp/.h   # Proxy configuration
│   │   └── device_registry.cpp/.h # Device management
│   ├── ui/                        # User interface
│   │   ├── app_window.slint       # UI definition
│   │   └── ui_logic.cpp/.h        # UI event handling
│   ├── utils/                     # Utilities
│   │   ├── logger.cpp/.h          # Logging system
│   │   ├── system_utils.cpp/.h    # System operations
│   │   └── translations.cpp/.h    # Internationalization
│   └── assets/                    # Application assets
│       └── logo ict.svg           # ICT Society logo
├── docs/                          # Documentation
├── build_x86/                     # 32-bit build output
├── build_x64/                     # 64-bit build output
├── CMakeLists.txt                 # Build configuration
└── *.bat                         # Build scripts
```

## Core Components

### 1. Application Entry Point (`main.cpp`)

**Responsibilities:**
- Initialize the application
- Set up signal handlers
- Create and configure the UI
- Connect UI callbacks to business logic
- Handle application lifecycle

**Key Features:**
- Exception handling for graceful error recovery
- Signal handling for clean shutdown
- Memory management with smart pointers

```cpp
int main(int argc, char** argv) {
    try {
        // Initialize logging and system checks
        LOG(T("app_version"));
        
        // Create UI and business logic
        auto app = AppWindow::create();
        auto logic = std::make_shared<UILogic>(&*app);
        
        // Connect callbacks with weak references for safety
        app->on_complete_setup([weak_logic]() { 
            if (auto l = weak_logic.lock()) l->complete_setup(); 
        });
        
        // Run the application
        app->run();
        
    } catch (const std::exception& e) {
        LOG("Fatal error: " + std::string(e.what()));
        return 1;
    }
}
```

### 2. User Interface Layer

#### Slint UI Framework (`app_window.slint`)
- **Declarative UI** definition using Slint markup language
- **Reactive properties** for data binding
- **Component-based** architecture for reusability
- **Cross-platform** rendering with native look and feel

#### UI Logic (`ui_logic.cpp/.h`)
- **Event handling** for UI interactions
- **State management** for application state
- **Thread coordination** between UI and background operations
- **Callback management** with proper lifetime handling

**Key Design Patterns:**
- **Observer Pattern** for UI updates
- **Command Pattern** for user actions
- **State Pattern** for application states

### 3. Network Management Layer

#### WiFi Manager (`wifi_manager.cpp/.h`)
**Responsibilities:**
- Enumerate available WiFi networks
- Connect to UNESWA networks using student credentials
- Manage WiFi profiles and saved connections
- Handle authentication protocols (WPA2/WPA3)

**Key APIs Used:**
- Windows WLAN API (`wlanapi.h`)
- Native WiFi functions for network operations
- Windows Credential Manager for secure storage

```cpp
class WiFiManager {
public:
    static WiFiResult connect(const WiFiCredentials& credentials);
    static bool is_connected();
    static std::vector<NetworkInfo> scan_networks();
    static WiFiResult remove_profile();
    
private:
    static bool authenticate_with_portal(const std::string& student_id, 
                                       const std::string& password);
};
```

#### Proxy Manager (`proxy_manager.cpp/.h`)
**Responsibilities:**
- Configure system proxy settings
- Handle university proxy authentication
- Manage proxy bypass rules
- Restore original proxy settings

**Implementation Details:**
- Uses Windows Registry API for proxy configuration
- Handles Internet Explorer proxy settings (system-wide)
- Supports automatic proxy configuration (PAC files)

#### Device Registry (`device_registry.cpp/.h`)
**Responsibilities:**
- Register devices with university systems
- Manage device authentication tokens
- Handle device-specific configurations
- Track registration status

### 4. Utility Layer

#### Logger (`logger.cpp/.h`)
**Features:**
- **Thread-safe** logging with mutex protection
- **Multiple output targets** (console, file, UI)
- **Log levels** for filtering messages
- **Callback system** for real-time UI updates

```cpp
class Logger {
public:
    static Logger& instance();
    void log(const std::string& message);
    void set_callback(std::function<void(std::string_view)> callback);
    
private:
    std::mutex mutex_;
    std::function<void(std::string_view)> ui_callback_;
};
```

#### System Utils (`system_utils.cpp/.h`)
**Capabilities:**
- **Administrator privilege** detection and elevation
- **System information** gathering (OS version, architecture)
- **Network adapter** enumeration and status
- **Process management** utilities

#### Translations (`translations.cpp/.h`)
**Features:**
- **Multi-language support** (English, siSwati)
- **Runtime language switching**
- **Fallback mechanism** for missing translations
- **Unicode support** for international characters

## Data Flow Architecture

### 1. User Interaction Flow
```
User Input → Slint UI → UI Logic → Business Logic → System APIs
     ↑                                                    ↓
UI Updates ← UI Logic ← Callbacks ← Results ← System Response
```

### 2. Network Connection Flow
```
1. User enters credentials in UI
2. UI Logic validates input
3. WiFi Manager attempts connection
4. System APIs handle network operations
5. Results propagated back through callbacks
6. UI updated with connection status
```

### 3. Threading Model
```
Main Thread (UI)
├── UI Event Loop (Slint)
├── UI Logic (Event Handlers)
└── Background Threads
    ├── Network Operations
    ├── Device Registration
    └── System Monitoring
```

## Security Considerations

### 1. Credential Handling
- **No plaintext storage** of passwords
- **Memory clearing** after use
- **Windows Credential Manager** integration
- **Secure transmission** to authentication endpoints

### 2. Network Security
- **Certificate validation** for HTTPS connections
- **Proxy authentication** handling
- **Network isolation** for sensitive operations
- **Timeout mechanisms** to prevent hanging

### 3. Application Security
- **Input validation** for all user inputs
- **Buffer overflow protection** with modern C++
- **Exception handling** to prevent crashes
- **Privilege escalation** only when necessary

## Performance Optimizations

### 1. Memory Management
- **Smart pointers** for automatic memory management
- **RAII principles** for resource cleanup
- **Move semantics** for efficient data transfer
- **Minimal copying** of large objects

### 2. Threading Optimizations
- **Background operations** for network calls
- **Thread pooling** for concurrent operations
- **Lock-free operations** where possible
- **Async callbacks** for UI responsiveness

### 3. UI Performance
- **Lazy loading** of UI components
- **Efficient data binding** with Slint
- **Minimal redraws** through state management
- **Resource caching** for images and assets

## Build System Architecture

### CMake Configuration
```cmake
# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Multi-architecture support
option(BUILD_32BIT "Build for 32-bit architecture" OFF)
if(BUILD_32BIT)
    set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

# Dependency management with FetchContent
FetchContent_Declare(Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    GIT_TAG release/1
)
FetchContent_Declare(cpr 
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)

# Target configuration
add_executable(AutoConnect ${SOURCES})
slint_target_sources(AutoConnect src/ui/app_window.slint)
target_link_libraries(AutoConnect PRIVATE Slint::Slint cpr::cpr wininet wlanapi)
```

### Dependency Management
- **FetchContent** for automatic dependency downloading
- **Version pinning** for reproducible builds
- **Cross-compilation** support for x86/x64
- **Administrator privileges** required for WiFi operations

## Testing Architecture

### Unit Testing Strategy
```cpp
// Component isolation for testing
class WiFiManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Mock system dependencies
        mock_wlan_api_ = std::make_unique<MockWlanApi>();
        wifi_manager_ = std::make_unique<WiFiManager>(mock_wlan_api_.get());
    }
};
```

### Integration Testing
- **End-to-end** workflow testing
- **Network simulation** for offline testing
- **UI automation** with Slint testing framework
- **Cross-platform** compatibility testing

## Error Handling Strategy

### 1. Exception Hierarchy
```cpp
class AutoConnectException : public std::runtime_error {
public:
    explicit AutoConnectException(const std::string& message);
};

class NetworkException : public AutoConnectException {
    // Network-specific errors
};

class AuthenticationException : public AutoConnectException {
    // Authentication failures
};
```

### 2. Result Types
```cpp
template<typename T>
struct Result {
    bool success;
    T value;
    std::string error_message;
    std::optional<ErrorCode> error_code;
};
```

### 3. Error Recovery
- **Graceful degradation** for non-critical failures
- **Retry mechanisms** with exponential backoff
- **User notification** for actionable errors
- **Logging** for debugging and support

## Future Architecture Considerations

### Planned Enhancements
1. **Plugin Architecture** for extensible network support
2. **Configuration Management** with JSON/YAML files
3. **Update Mechanism** for automatic application updates
4. **Telemetry System** for usage analytics (opt-in)
5. **Cross-Platform Support** (macOS, Linux)

### Scalability Considerations
- **Microservice Architecture** for cloud integration
- **Database Integration** for user management
- **API Gateway** for external service integration
- **Containerization** for deployment flexibility

---

This architecture provides a solid foundation for the UNESWA WiFi AutoConnect application while maintaining flexibility for future enhancements and platform expansion. 