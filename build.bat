@echo off

@call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
Set CPU=i386
Set TARGETOS=WINNT
Set APPVER=5.01
@call "%DXSDK_DIR%\Utilities\bin\dx_setenv.cmd" x86


configure.exe -buildkey Cedrus-Qt-4.7.0 -debug-and-release -opensource -platform win32-msvc2010 -webkit -phonon -phonon-backend -mmx -3dnow -sse -sse2 -vcproj -nomake demos -nomake examples

nmake /NOLOGOO

%EXTERNAL_SDKS%\setx\setx.exe QTDIR %~dp0 -m
