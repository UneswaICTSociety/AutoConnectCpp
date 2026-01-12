# UNESWA WiFi AutoConnect - Style Guide

This document outlines the coding standards and style conventions for the UNESWA WiFi AutoConnect project. Following these guidelines ensures consistency, readability, and maintainability across the codebase.

## General Principles

- **Clarity over cleverness** - Write code that's easy to understand
- **Consistency** - Follow established patterns in the codebase
- **Simplicity** - Prefer simple solutions over complex ones
- **Performance** - Consider performance implications, especially for UI operations
- **Safety** - Use modern C++ features for memory and thread safety

## C++ Style Guidelines

### File Organization

#### Header Files (.h)
```cpp
#pragma once

// System includes first
#include <string>
#include <memory>
#include <vector>

// Third-party includes
#include <slint.h>

// Project includes
#include "utils/logger.h"

// Forward declarations when possible
class WiFiManager;

class MyClass {
public:
    // Public interface first
    MyClass();
    ~MyClass();
    
    void public_method();
    
private:
    // Private members last
    std::string member_variable_;
    
    void private_method();
};
```

#### Source Files (.cpp)
```cpp
#include "my_class.h"

// System includes
#include <iostream>
#include <algorithm>

// Third-party includes
#include <nlohmann/json.hpp>

// Project includes
#include "network/wifi_manager.h"
#include "utils/system_utils.h"

// Implementation
MyClass::MyClass() : member_variable_("default") {
    // Constructor implementation
}
```

### Naming Conventions

#### Classes and Structs
```cpp
// PascalCase for class names
class WiFiManager {
    // ...
};

struct NetworkCredentials {
    // ...
};
```

#### Functions and Methods
```cpp
// snake_case for functions and methods
void connect_to_network();
bool is_connected() const;
std::string get_network_name();

// Exception: Slint callback methods can use camelCase to match UI
void onButtonClicked();
```

#### Variables
```cpp
// snake_case for variables
std::string student_id;
bool is_working = false;
int connection_timeout = 30;

// Member variables end with underscore
class MyClass {
private:
    std::string network_name_;
    bool is_connected_;
};
```

#### Constants
```cpp
// UPPER_SNAKE_CASE for constants
const int MAX_RETRY_ATTEMPTS = 3;
const std::string DEFAULT_NETWORK_NAME = "UNESWA_WiFi";

// Or use constexpr for compile-time constants
constexpr int BUFFER_SIZE = 1024;
```

#### Enums
```cpp
// PascalCase for enum class names
enum class ConnectionStatus {
    DISCONNECTED,    // UPPER_SNAKE_CASE for values
    CONNECTING,
    CONNECTED,
    FAILED
};
```

### Code Formatting

#### Indentation and Spacing
- Use **4 spaces** for indentation (no tabs)
- **80-100 characters** per line (prefer 80, allow up to 100)
- **One space** around operators: `a + b`, `x == y`
- **No space** before parentheses in function calls: `function(args)`
- **Space after** control keywords: `if (condition)`, `for (init; cond; inc)`

#### Braces
```cpp
// Opening brace on same line for functions, classes, control structures
class MyClass {
public:
    void my_function() {
        if (condition) {
            do_something();
        } else {
            do_something_else();
        }
    }
};

// Exception: Long function signatures can break before brace
void very_long_function_name_with_many_parameters(
    const std::string& first_parameter,
    int second_parameter,
    bool third_parameter)
{
    // Implementation
}
```

#### Function Parameters
```cpp
// Short parameter lists on one line
void connect(const std::string& ssid, const std::string& password);

// Long parameter lists - one per line, aligned
void configure_network(
    const std::string& network_name,
    const std::string& student_id,
    const std::string& password,
    bool enable_proxy,
    int timeout_seconds);
```

### Modern C++ Features

#### Use C++17 Features
```cpp
// Use auto for type deduction when type is obvious
auto connection = std::make_unique<WiFiConnection>();
auto result = network_manager.connect();

// Use range-based for loops
for (const auto& network : available_networks) {
    process_network(network);
}

// Use structured bindings (C++17)
auto [success, message] = attempt_connection();

// Use std::optional for optional values (C++17)
std::optional<std::string> get_saved_password(const std::string& network);
```

#### Smart Pointers
```cpp
// Prefer smart pointers over raw pointers
std::unique_ptr<WiFiManager> wifi_manager_;
std::shared_ptr<Logger> logger_;

// Use make_unique and make_shared
auto manager = std::make_unique<WiFiManager>();
auto logger = std::make_shared<Logger>();
```

#### RAII and Resource Management
```cpp
class NetworkConnection {
public:
    NetworkConnection(const std::string& ssid) : ssid_(ssid) {
        // Acquire resources in constructor
        connect();
    }
    
    ~NetworkConnection() {
        // Release resources in destructor
        disconnect();
    }
    
    // Delete copy operations if not needed
    NetworkConnection(const NetworkConnection&) = delete;
    NetworkConnection& operator=(const NetworkConnection&) = delete;
    
    // Move operations if needed
    NetworkConnection(NetworkConnection&&) = default;
    NetworkConnection& operator=(NetworkConnection&&) = default;
    
private:
    std::string ssid_;
};
```

### Error Handling

#### Use Exceptions for Exceptional Cases
```cpp
class NetworkException : public std::runtime_error {
public:
    explicit NetworkException(const std::string& message) 
        : std::runtime_error(message) {}
};

void connect_to_network() {
    if (!wifi_adapter_available()) {
        throw NetworkException("No WiFi adapter found");
    }
    // ... connection logic
}
```

#### Use Result Types for Expected Failures
```cpp
struct ConnectionResult {
    bool success;
    std::string message;
    std::optional<std::string> error_code;
};

ConnectionResult attempt_connection(const Credentials& creds) {
    // ... attempt connection
    if (failed) {
        return {false, "Invalid credentials", "AUTH_FAILED"};
    }
    return {true, "Connected successfully", std::nullopt};
}
```

### Thread Safety

#### Use Mutexes for Shared Data
```cpp
class ThreadSafeLogger {
private:
    mutable std::mutex mutex_;
    std::vector<std::string> log_entries_;
    
public:
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        log_entries_.push_back(message);
    }
    
    std::vector<std::string> get_logs() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return log_entries_;  // Copy
    }
};
```

#### Use std::atomic for Simple Types
```cpp
class ConnectionManager {
private:
    std::atomic<bool> is_connected_{false};
    std::atomic<int> retry_count_{0};
    
public:
    bool is_connected() const { return is_connected_.load(); }
    void set_connected(bool connected) { is_connected_.store(connected); }
};
```

## Slint UI Style Guidelines

### File Organization
```slint
// app_window.slint
import { Button, LineEdit, VerticalBox } from "std-widgets.slint";

// Component definitions
component CredentialInput inherits VerticalBox {
    property <string> label;
    property <string> placeholder;
    property <string> text;
    
    Text {
        text: root.label;
        font-weight: 600;
    }
    
    LineEdit {
        placeholder-text: root.placeholder;
        text <=> root.text;
    }
}

// Main window
export component AppWindow inherits Window {
    // Properties first
    property <string> student-id;
    property <string> birthday;
    
    // Callbacks
    callback complete-setup();
    callback quit-app();
    
    // Layout
    VerticalBox {
        padding: 20px;
        spacing: 15px;
        
        // UI elements
    }
}
```

### Naming in Slint
- Use **kebab-case** for properties and callbacks: `student-id`, `complete-setup`
- Use **PascalCase** for component names: `AppWindow`, `CredentialInput`
- Use **descriptive names** for UI elements

### Layout Guidelines
```slint
// Use consistent spacing and padding
VerticalBox {
    padding: 20px;
    spacing: 15px;
    
    // Group related elements
    HorizontalBox {
        spacing: 10px;
        
        Button {
            text: "Connect";
            preferred-width: 120px;
        }
        
        Button {
            text: "Cancel";
            preferred-width: 120px;
        }
    }
}
```

## Documentation Style

### Code Comments
```cpp
/**
 * @brief Connects to the specified WiFi network using student credentials
 * 
 * This function attempts to establish a connection to the UNESWA WiFi network
 * using the provided student ID and birthday. It handles authentication,
 * proxy configuration, and device registration.
 * 
 * @param credentials Student credentials containing ID and birthday/password
 * @return ConnectionResult indicating success/failure and details
 * 
 * @throws NetworkException if no WiFi adapter is available
 * @throws std::invalid_argument if credentials are empty
 */
ConnectionResult connect_to_network(const StudentCredentials& credentials);

// Use single-line comments for brief explanations
void update_ui() {
    // Refresh connection status indicators
    update_status_icons();
    
    // Update log display with latest entries
    refresh_log_display();
}
```

### README and Documentation
- Use **clear headings** for organization
- Include **code examples** for complex procedures
- Provide **step-by-step instructions** for setup
- Use **bullet points** and **numbered lists** for readability

## Testing Style

### Unit Test Structure
```cpp
#include <gtest/gtest.h>
#include "network/wifi_manager.h"

class WiFiManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test environment
        wifi_manager_ = std::make_unique<WiFiManager>();
    }
    
    void TearDown() override {
        // Cleanup
        wifi_manager_.reset();
    }
    
    std::unique_ptr<WiFiManager> wifi_manager_;
};

TEST_F(WiFiManagerTest, ConnectWithValidCredentials) {
    // Arrange
    StudentCredentials valid_creds{"12345", "01011990"};
    
    // Act
    auto result = wifi_manager_->connect(valid_creds);
    
    // Assert
    EXPECT_TRUE(result.success);
    EXPECT_FALSE(result.message.empty());
}

TEST_F(WiFiManagerTest, ConnectWithInvalidCredentials) {
    // Arrange
    StudentCredentials invalid_creds{"", ""};
    
    // Act & Assert
    EXPECT_THROW(wifi_manager_->connect(invalid_creds), std::invalid_argument);
}
```

## Build and Configuration

### CMakeLists.txt Style
```cmake
# Clear project definition
project(AutoConnect LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Group related settings
option(BUILD_32BIT "Build for 32-bit architecture" OFF)
if(BUILD_32BIT)
    set(CMAKE_GENERATOR_PLATFORM Win32)
endif()

# Organize source files logically
set(SOURCES
    src/main.cpp
    src/network/wifi_manager.cpp
    src/network/proxy_manager.cpp
    src/ui/ui_logic.cpp
    src/utils/logger.cpp
)

# Clear target definition
add_executable(AutoConnect ${SOURCES})

# Group dependencies
target_link_libraries(AutoConnect
    PRIVATE
    Slint::Slint
    cpr::cpr
    wininet
    wlanapi
)
```

## Pre-commit Checklist

Before committing code, ensure:

- [ ] **Code compiles** without warnings on both x86 and x64
- [ ] **Tests pass** (when available)
- [ ] **Code follows** naming conventions
- [ ] **Functions are documented** with clear comments
- [ ] **No memory leaks** or resource leaks
- [ ] **Thread safety** considered for concurrent code
- [ ] **Error handling** is appropriate
- [ ] **UI changes** tested on different screen sizes

## Tools and IDE Setup

### Visual Studio 2022 Configuration
- Install **C++ CMake tools**
- Enable **ClangFormat** for automatic formatting
- Configure **IntelliSense** for better code completion
- Use **Code Analysis** to catch potential issues

### Recommended Extensions
- **CMake Tools** for CMake integration
- **Rust Analyzer** for Rust code (Slint compilation)
- **GitLens** for Git integration
- **Error Lens** for inline error display

---

Remember: Good code is written for humans to read, not just for computers to execute!