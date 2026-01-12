cmake_minimum_required(VERSION 3.21)
project(AutoConnect LANGUAGES CXX)

# Option to build for 32-bit
option(BUILD_32BIT "Build for 32-bit architecture" OFF)

# Set architecture
if(BUILD_32BIT)
    message(STATUS "Building for 32-bit architecture")
    set(CMAKE_GENERATOR_PLATFORM Win32)
    set(CMAKE_SIZEOF_VOID_P 4)
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:IA32")
    endif()
else()
    message(STATUS "Building for 64-bit architecture (default)")
endif()

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Slint QUIET)
if (NOT Slint_FOUND)
  message("Slint could not be located in the CMake module search path. Downloading it from Git and building it locally")
  include(FetchContent)
  FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    # `release/1` will auto-upgrade to the latest Slint >= 1.0.0 and < 2.0.0
    # `release/1.0` will auto-upgrade to the latest Slint >= 1.0.0 and < 1.1.0
    GIT_TAG release/1
    SOURCE_SUBDIR api/cpp
  )
  FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

# Add CPR library for HTTP requests
include(FetchContent)
FetchContent_Declare(cpr 
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.5
)
FetchContent_MakeAvailable(cpr)

# Shared source files
set(SHARED_SOURCES
    src/network/device_registry.cpp
    src/network/proxy_manager.cpp
    src/network/wifi_manager.cpp
    src/utils/logger.cpp
    src/utils/system_utils.cpp
    src/utils/translations.cpp
)

# GUI Application
add_executable(AutoConnect 
    src/main.cpp
    src/ui/ui_logic.cpp
    ${SHARED_SOURCES}
)

target_link_libraries(AutoConnect PRIVATE Slint::Slint cpr::cpr wininet wlanapi)
slint_target_sources(AutoConnect src/ui/app_window.slint)

if(WIN32 AND MSVC)
    set_target_properties(AutoConnect PROPERTIES
        LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\""
    )
endif()

if (WIN32)
    add_custom_command(TARGET AutoConnect POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:AutoConnect> $<TARGET_FILE_DIR:AutoConnect> COMMAND_EXPAND_LISTS)
    add_custom_command(TARGET AutoConnect POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/assets $<TARGET_FILE_DIR:AutoConnect>/assets)
endif()