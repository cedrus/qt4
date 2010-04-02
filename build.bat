@echo off

@call "%VS80COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

configure.exe -buildkey Cedrus-Qt-4.6.2 -debug-and-release -opensource -platform win32-msvc2005 -webkit -phonon -phonon-backend -mmx -3dnow -sse -sse2 -vcproj

nmake

%EXTERNAL_SDKS%\setx\setx.exe QTDIR %~dp0 -m
