@echo off
echo Creating NSIS installer for x64 (64-bit) version...

set MAKENSIS="C:\Program Files (x86)\NSIS\makensis.exe"
if not exist %MAKENSIS% (
    where makensis >nul 2>nul
    if %errorlevel% neq 0 (
        echo NSIS not found. Please install NSIS.
        exit /b 1
    )
    set MAKENSIS=makensis
)

REM Check if build exists
if not exist "build_x64\Release\AutoConnect.exe" (
    echo x64 build not found! Please run build_x64.bat first.
    exit /b 1
)

REM Check if required files exist
if not exist "LICENSE" (
    echo LICENSE file not found!
    exit /b 1
)

if not exist "README.md" (
    echo README.md file not found!
    exit /b 1
)

%MAKENSIS% installer_x64.nsi

if %errorlevel% neq 0 (
    echo Installer creation failed!
    exit /b 1
)

echo.
echo ✓ x64 installer created successfully!
echo Output: UNESWA_AutoConnect_Setup_x64.exe

if exist "UNESWA_AutoConnect_Setup_x64.exe" (
    echo File size: 
    for %%A in ("UNESWA_AutoConnect_Setup_x64.exe") do echo %%~zA bytes
)