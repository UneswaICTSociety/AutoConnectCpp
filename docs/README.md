# UNESWA WiFi AutoConnect - User Guide

Automated WiFi connection tool for University of Eswatini students.

A modern C++ application with Slint UI that automatically connects UNESWA students to the university WiFi network using thier student credentials.

## Features

- **Automatic WiFi Connection** - Seamlessly connects to UNESWA WiFi networks
- **Student Credential Management** - Secure handling of student ID and birthday authentication  
- **Password Mode Support** - Option to use custom password instead of birthday
- **Modern GUI** - Built with Slint UI framework for a clean, responsive interface
- **Cross-Architecture Support** - Available for both 32-bit and 64-bit Windows systems
- **Network Detection** - Automatically detects and connects to available UNESWA networks
- **Proxy Management** - Handles university proxy settings automatically
- **Device Registry** - Manages known devices and connection preferances
- **Multi-language Support** - Available in English and siSwati

## Download and Installation

### Ready-to-Use Installers

Download the appropriate installer for your system:

- **32-bit Windows (x86)** - Compatible with all Windows systems
- **64-bit Windows (x64)** - Optimized for 64-bit systems

### Installation Steps

1. Download the appropriate installer
2. Double-click the `.exe` file
3. Follow the installation wizard (Next → Next → Install)
4. Launch the application from Desktop or Start Menu

**No additional dependencies required!** All necessary libraries are included in the installer.

## System Requirements

- **Operating System**: Windows 10 or later
- **Architecture**: 32-bit or 64-bit
- **Memory**: 50 MB available space
- **Network**: WiFi adapter
- **Privileges**: Administrator rights required

## Usage

1. **Launch** the application
2. **Enter** your student ID 
3. **Choose password mode** - Birthday (default) or Custom Password
4. **Enter** your birthday (ddmmyyyy format) or custom password
5. **Click Connect** to automatically join the UNESWA WiFi
6. **Stay Connected** - the app will remeber your preferences

## Building from Source

### Prerequisites

- **Visual Studio 2022** with C++ development tools
- **CMake** 3.21 or higher
- **Rust toolchain** with targets:
  - `stable-i686-pc-windows-msvc` (for 32-bit)
  - `stable-x86_64-pc-windows-msvc` (for 64-bit)
- **NSIS** (for creating installers)

### Build Commands

```bash
# Install Rust targets
rustup target add i686-pc-windows-msvc
rustup target add x86_64-pc-windows-msvc

# Build individually
build.bat x86          # 32-bit version
build.bat x64          # 64-bit version (default)

# Create installers after building
create_installer_x86.bat
create_installer_x64.bat
```

## Architecture

```
src/
├── main.cpp                    # Application entry point
├── network/                    # Network management
│   ├── wifi_manager.cpp       # WiFi connection handling
│   ├── proxy_manager.cpp      # Proxy configuration
│   └── device_registry.cpp    # Device management
├── ui/                        # User interface
│   ├── app_window.slint       # UI definition
│   └── ui_logic.cpp          # UI event handling
└── utils/                     # Utilities
    ├── logger.cpp             # Logging system
    ├── system_utils.cpp       # System operations
    └── translations.cpp       # Internationalization
```

## Contributing

We welcome contributions from the UNESWA community!

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Commit** your changes (`git commit -m 'Add amazing feature'`)
4. **Push** to the branch (`git push origin feature/amazing-feature`)
5. **Open** a Pull Request

## Development Guidelines

- Follow C++17 standards
- Use meaningful commit messages
- Test on both 32-bit and 64-bit systems
- Update documentation for new features

## Issues and Support

- **Bug Reports**: Create an issue for bug reports
- **Feature Requests**: Submit feature requests  
- **Questions**: Ask questions in discussions

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## About

**Developed by the ICT Society - University of Eswatini**

Solution for students by students

This project aims to simplify the WiFi connection process for UNESWA students, eliminating the need for manual network configuration and providing a seamless internet access experience on campus.

---

Made with care for the UNESWA community