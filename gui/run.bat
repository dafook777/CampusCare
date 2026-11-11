@echo off
setlocal enabledelayedexpansion

set "QT_BIN=C:\Qt\6.12.0\mingw_64\bin"
set "QT_PLUGIN_PATH=C:\Qt\6.12.0\mingw_64\plugins"
set "MINGW_BIN=C:\Qt\Tools\mingw1310_64\bin"
for /d %%D in (C:\Qt\Tools\mingw*\bin) do (
    if exist "%%D" set "MINGW_BIN=%%D"
)

set "PATH=%QT_BIN%;%MINGW_BIN%;C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;%PATH%"

set "GUI_DIR=%~dp0"
set "BUILD_DIR=%GUI_DIR%build"
set "EXE_NAME=CampusCareGUI.exe"

if not exist "%BUILD_DIR%\%EXE_NAME%" (
    echo [*] Executable not found. Building project...
    cd /d "%GUI_DIR%"
    cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=C:/Qt/6.12.0/mingw_64 -DCMAKE_BUILD_TYPE=Release
    if errorlevel 1 (
        echo [ERROR] CMake configuration failed.
        pause
        exit /b 1
    )
    cmake --build build
    if errorlevel 1 (
        echo [ERROR] Build failed.
        pause
        exit /b 1
    )
)

cd /d "%BUILD_DIR%"
start "" "%BUILD_DIR%\%EXE_NAME%"
if errorlevel 1 (
    echo [ERROR] Failed to launch application.
    pause
    exit /b 1
)
