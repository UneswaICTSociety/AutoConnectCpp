; NSIS Installer Script for UNESWA WiFi AutoConnect (x86/32-bit)
; ICT Society - University of Eswatini
; Solution for students by students

!define APP_NAME "UNESWA WiFi AutoConnect"
!define APP_VERSION "1.3.7"
!define APP_PUBLISHER "ICT Society - University of Eswatini"
!define APP_URL ""
!define APP_DESCRIPTION "Automated WiFi connection tool for UNESWA students (32-bit)"
!define ARCH "x86"

; Installer properties
Name "${APP_NAME} (${ARCH})"
OutFile "UNESWA_AutoConnect_Setup_x86.exe"
InstallDir "$LOCALAPPDATA\${APP_NAME}"
InstallDirRegKey HKCU "Software\${APP_NAME}" "InstallDir"
RequestExecutionLevel user

; Modern UI
!include "MUI2.nsh"

; Interface Settings
!define MUI_ABORTWARNING

; Welcome page
!define MUI_WELCOMEPAGE_TITLE "Welcome to ${APP_NAME} Setup"
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of ${APP_NAME} (32-bit version).$\r$\n$\r$\nSolution for students by students - ICT Society, University of Eswatini.$\r$\n$\r$\nClick Next to continue."
!insertmacro MUI_PAGE_WELCOME

; License page
!define MUI_LICENSEPAGE_TEXT_TOP "Please read the following License Agreement. You must accept the terms of this agreement before continuing with the installation."
!define MUI_LICENSEPAGE_TEXT_BOTTOM "If you accept the terms of the agreement, click I accept the agreement to continue. You must accept the agreement to install ${APP_NAME}."
!insertmacro MUI_PAGE_LICENSE "LICENSE"

; Components page
!insertmacro MUI_PAGE_COMPONENTS

; Directory page
!define MUI_DIRECTORYPAGE_TEXT_TOP "Setup will install ${APP_NAME} in the following folder. To install in a different folder, click Browse and select another folder. Click Next to continue."
!insertmacro MUI_PAGE_DIRECTORY

; Installation page
!insertmacro MUI_PAGE_INSTFILES

; Finish page
!define MUI_FINISHPAGE_TITLE "Completing the ${APP_NAME} Setup Wizard"
!define MUI_FINISHPAGE_TEXT "${APP_NAME} has been installed on your computer.$\r$\n$\r$\nClick Finish to close this wizard."
!define MUI_FINISHPAGE_RUN "$INSTDIR\AutoConnect.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Launch ${APP_NAME}"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.md"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Show README"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

; Version Information
VIProductVersion "1.3.7.0"
VIAddVersionKey "ProductName" "${APP_NAME} (${ARCH})"
VIAddVersionKey "ProductVersion" "${APP_VERSION}"
VIAddVersionKey "CompanyName" "${APP_PUBLISHER}"
VIAddVersionKey "FileDescription" "${APP_DESCRIPTION}"
VIAddVersionKey "FileVersion" "${APP_VERSION}"
VIAddVersionKey "LegalCopyright" "© 2026 ICT Society - University of Eswatini"

; Installer sections
Section "!${APP_NAME} (required)" SecMain
  SectionIn RO
  
  ; Set output path to the installation directory
  SetOutPath $INSTDIR
  
  ; Copy main executable (32-bit build)
  File "build_x86\Release\AutoConnect.exe"
  
  ; Copy 32-bit DLLs
  File "build_x86\Release\slint_cpp.dll"
  File "build_x86\Release\libcurl.dll"
  File "build_x86\Release\cpr.dll"
  
  ; Copy documentation
  File "LICENSE"
  File "README.md"
  
  ; Store installation folder
  WriteRegStr HKCU "Software\${APP_NAME}" "InstallDir" $INSTDIR
  WriteRegStr HKCU "Software\${APP_NAME}" "Architecture" "${ARCH}"
  
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  ; Add to Add/Remove Programs
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "DisplayName" "${APP_NAME} (${ARCH})"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "InstallLocation" "$INSTDIR"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "DisplayIcon" "$INSTDIR\AutoConnect.exe"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "Publisher" "${APP_PUBLISHER}"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "DisplayVersion" "${APP_VERSION}"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "URLInfoAbout" "${APP_URL}"
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}" "NoRepair" 1
SectionEnd

Section "Desktop Shortcut" SecDesktop
  CreateShortcut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\AutoConnect.exe" "" "$INSTDIR\AutoConnect.exe" 0 SW_SHOWNORMAL "" "${APP_DESCRIPTION}"
SectionEnd

Section "Start Menu Shortcuts" SecStartMenu
  CreateDirectory "$SMPROGRAMS\${APP_NAME}"
  CreateShortcut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\AutoConnect.exe" "" "$INSTDIR\AutoConnect.exe" 0 SW_SHOWNORMAL "" "${APP_DESCRIPTION}"
  CreateShortcut "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0 SW_SHOWNORMAL "" "Uninstall ${APP_NAME}"
SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "Core application files (required)"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktop} "Create a shortcut on the desktop"
  !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} "Create shortcuts in the Start Menu"
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Uninstaller section
Section "Uninstall"
  ; Remove registry keys
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}_${ARCH}"
  DeleteRegKey HKCU "Software\${APP_NAME}"
  
  ; Remove files and uninstaller
  Delete "$INSTDIR\AutoConnect.exe"
  Delete "$INSTDIR\slint_cpp.dll"
  Delete "$INSTDIR\libcurl.dll"
  Delete "$INSTDIR\cpr.dll"
  Delete "$INSTDIR\LICENSE"
  Delete "$INSTDIR\README.md"
  Delete "$INSTDIR\Uninstall.exe"
  
  ; Remove shortcuts
  Delete "$DESKTOP\${APP_NAME}.lnk"
  Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
  Delete "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk"
  RMDir "$SMPROGRAMS\${APP_NAME}"
  
  ; Remove installation directory if empty
  RMDir "$INSTDIR"
SectionEnd

; Functions
Function .onInit
  ; Check if already installed
  ReadRegStr $R0 HKCU "Software\${APP_NAME}" "InstallDir"
  StrCmp $R0 "" done
  
  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "${APP_NAME} is already installed. $\n$\nClick 'OK' to remove the previous version or 'Cancel' to cancel this upgrade." \
  IDOK uninst
  Abort
  
uninst:
  ; Run the uninstaller
  ClearErrors
  ExecWait '$R0\Uninstall.exe _?=$R0'
  
  IfErrors no_remove_uninstaller done
  no_remove_uninstaller:
  
done:
FunctionEnd