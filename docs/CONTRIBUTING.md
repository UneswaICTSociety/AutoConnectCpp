# Contributing to UNESWA WiFi AutoConnect

Thanks for your interest in contributing to the UNESWA WiFi AutoConnect project! This guide will help you get started with contributing to our codebase.

## Project Goals

Our mission is to provide a seamless WiFi connection experience for UNESWA students through:
- **Reliability** - Consistent and stable connections
- **Simplicity** - Easy-to-use interface for all technical levels
- **Performance** - Fast connection times and minimal resource usage
- **Accessibility** - Support for multiple languages and system configurations

## How to Contribute

### Getting Started

1. **Fork** the repository on GitHub
2. **Clone** your fork locally:
   ```bash
   git clone https://github.com/your-username/AutoConnectCpp.git
   cd AutoConnectCpp
   ```
3. **Set up** the development environment (see Development Setup below)
4. **Create** a new branch for your feature:
   ```bash
   git checkout -b feature/your-feature-name
   ```

### Types of Contributions

We welcome various types of contributions:

#### Bug Fixes
- Fix crashes, connection issues, or UI problems
- Improve error handling and user feedback
- Resolve compatibility issues

#### New Features
- Add support for new network types
- Implement additional authentication methods
- Create new UI components or improve existing ones
- Add internationalization support

#### Documentation
- Improve code comments and documentation
- Write user guides and tutorials
- Create developer documentation
- Update README files

#### Testing
- Write unit tests for new features
- Improve test coverage
- Test on different Windows versions
- Report and verify bug fixes

### Development Setup

#### Prerequisites
- **Visual Studio 2022** with C++ development workload
- **CMake** 3.21 or higher
- **Git** for version control
- **Rust toolchain** with Windows targets:
  ```bash
  rustup target add i686-pc-windows-msvc
  rustup target add x86_64-pc-windows-msvc
  ```

#### Building the Project
```bash
# Configure and build 32-bit version
cmake -B build_x86 -A Win32
cmake --build build_x86 --config Release

# Configure and build 64-bit version
cmake -B build_x64 -A x64
cmake --build build_x64 --config Release
```

#### Running Tests
```bash
# Run unit tests (when available)
ctest --test-dir build_x86 --config Release
ctest --test-dir build_x64 --config Release
```

## Contribution Guidelines

### Code Style

Please follow our Style Guide for consistent code formatting and structure.

#### Key Points:
- Use **C++17** features where appropriate
- Follow **RAII** principles for resource management
- Use **meaningful variable and function names**
- Add **comments** for complex logic
- Keep functions **focused and small**

### Commit Messages

Use clear, descriptive commit messages following this format:

```
type(scope): brief description

Detailed explanation of what changed and why.

Fixes #issue-number
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

**Examples:**
```
feat(wifi): add support for WPA3 networks

Implement WPA3 authentication to support newer network
configurations used by the university.

Fixes #123

fix(ui): prevent crash when clicking buttons rapidly

Add mutex protection to UI callbacks to prevent race
conditions that could cause the application to crash.

Fixes #456
```

### Pull Request Process

1. **Update** your branch with the latest changes:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Test** your changes thoroughly:
   - Build both 32-bit and 64-bit versions
   - Test on different Windows versions if possible
   - Verify no regressions in existing functionality

3. **Create** a pull request with:
   - Clear title describing the change
   - Detailed description of what was changed and why
   - Screenshots for UI changes
   - Reference to related issues

4. **Respond** to code review feedback promptly
5. **Update** your PR based on reviewer suggestions

### Code Review Criteria

Pull requests will be evaluated on:
- **Functionality** - Does it work as intended?
- **Code Quality** - Is it well-written and maintainable?
- **Testing** - Are there appropriate tests?
- **Documentation** - Is it properly documented?
- **Compatibility** - Does it work on target platforms?

## Reporting Issues

### Bug Reports

When reporting bugs, please include:
- **Windows version** and architecture (32/64-bit)
- **Steps to reproduce** the issue
- **Expected behavior** vs actual behavior
- **Screenshots** or error messages
- **Log files** if available

Use our bug report template when creating issues.

### Feature Requests

For feature requests, please describe:
- **Problem** you're trying to solve
- **Proposed solution** or feature
- **Use case** and benefits
- **Alternative solutions** considered

## Architecture Overview

Understanding the project structure will help you contribute effectively:

```
src/
├── main.cpp                    # Application entry point
├── network/                    # Network management layer
│   ├── wifi_manager.cpp       # WiFi connection handling
│   ├── proxy_manager.cpp      # Proxy configuration
│   └── device_registry.cpp    # Device management
├── ui/                        # User interface layer
│   ├── app_window.slint       # UI definition (Slint markup)
│   ├── ui_logic.cpp          # UI event handling
│   └── ui_logic.h            # UI logic interface
└── utils/                     # Utility libraries
    ├── logger.cpp             # Logging system
    ├── system_utils.cpp       # System operations
    └── translations.cpp       # Internationalization
```

### Key Components

- **WiFiManager** - Handles all WiFi-related operations
- **ProxyManager** - Manages university proxy settings
- **DeviceRegistry** - Tracks registered devices
- **UILogic** - Bridges UI events with business logic
- **Logger** - Centralized logging system
- **Translations** - Multi-language support

## Internationalization

We support multiple languages:
- **English** (default)
- **siSwati** (Swazi)

To add translations:
1. Update `src/utils/translations.cpp`
2. Add new language constants
3. Test UI layout with longer text strings

## Testing Guidelines

### Manual Testing Checklist

Before submitting a PR, please test:
- [ ] Application starts without errors
- [ ] WiFi connection works with valid credentials
- [ ] Error handling for invalid credentials
- [ ] UI responsiveness and layout
- [ ] Language switching functionality
- [ ] Application closes cleanly

### Automated Testing

We're working on expanding our test suite. Consider adding:
- Unit tests for utility functions
- Integration tests for network operations
- UI automation tests

## Getting Help

If you need help or have questions:

1. **Check existing issues** and discussions
2. **Search documentation** in the `docs/` folder
3. **Ask questions** in discussions
4. **Contact maintainers** through issues

## Recognition

Contributors will be recognized in:
- **CONTRIBUTORS.md** file
- **Release notes** for significant contributions
- **GitHub contributors** section

## License

By contributing to this project, you agree that your contributions will be licensed under the MIT License.

---

Thank you for helping make UNESWA WiFi connection easier for everyone!