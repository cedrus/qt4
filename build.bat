@echo off

@call "C:\Program Files (x86)\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"

configure.exe -buildkey Cedrus-Qt-4.6.2 -debug-and-release -opensource -platform win32-msvc -webkit 

nmake

%EXTERNAL_SDKS%\setx\setx.exe QTDIR %~dp0 -m
