@echo off
rem RADIO DAYS - starts the Windows USB relay (RadioDaysRelay_U9_Windows.ps1, next to this file)
rem with Windows PowerShell 5.1, which comes with Windows 10 and 11. Double-click this file.
rem "-ExecutionPolicy Bypass" applies to this one run only: no setting of the PC is changed.
rem Arguments are passed on, e.g.  RadioDaysRelay_U9_Windows.cmd --port COM5
rem SPDX-License-Identifier: MIT
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0RadioDaysRelay_U9_Windows.ps1" %*
echo.
echo The relay has stopped. Press any key to close this window.
pause > nul
