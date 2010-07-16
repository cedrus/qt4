@echo off

@call "%VS100COMNTOOLS%\..\..\VC\vcvarsall.bat" x86
Set MSSdk=%SYSTEMDRIVE%\Program Files\Microsoft Platform SDK for Windows Server 2003 R2
Set Lib=%MSSdk%\Lib;%Lib%
Set Include=%MSSdk%\Include;%Include%
Set CPU=i386
Set TARGETOS=WINNT
Set APPVER=5.01
@call "%DXSDK_DIR%\Utilities\bin\dx_setenv.cmd" x86


configure.exe -buildkey Cedrus-Qt-4.6.2 -debug-and-release -opensource -platform win32-msvc2010 -webkit -phonon -phonon-backend -mmx -3dnow -sse -sse2 -vcproj 

REM nmake

%EXTERNAL_SDKS%\setx\setx.exe QTDIR %~dp0 -m
