@echo off
echo UNESWA AutoConnect - Build Script
echo ==================================

set ARCH=%1
if "%ARCH%"=="" set ARCH=x64

echo Building for %ARCH% architecture...

REM Clean previous build
REM if exist build_%ARCH% rmdir /s /q build_%ARCH%

REM Configure CMake
if "%ARCH%"=="x86" (
    cmake -B build_x86 -A Win32 -DCMAKE_BUILD_TYPE=Release -DBUILD_32BIT=ON
) else (
    cmake -B build_x64 -A x64 -DCMAKE_BUILD_TYPE=Release -DBUILD_32BIT=OFF
)

if %errorlevel% neq 0 (
    echo CMake configuration failed!
    exit /b 1
)

REM Build the project
cmake --build build_%ARCH% --config Release

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build completed successfully!
echo Executable: build_%ARCH%\Release\AutoConnect.exe
echo.
echo To create installer, run: create_installer_%ARCH%.bat