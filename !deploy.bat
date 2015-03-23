@echo off
copy src\base C:\!DLL\include\base
copy src\base.h C:\!DLL\include
copy lib C:\!DLL\lib

if "%1"=="np" goto END
pause
:END
