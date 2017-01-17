@echo off
SET RDP_PORT=%1
SET NAGIOSHOST1=%2
SET NAGIOSHOST2=%3
SET NAGIOSHOST3=%4
IF NOT DEFINED RDP_PORT GOTO :defsettings
IF %RDP_PORT% EQU "-h" GOTO :usage
GOTO check

:defsettings
set RDP_PORT=3389

:check
netstat -a -n | find "%RDP_PORT%" | find "LISTENING" > NUL
IF %ERRORLEVEL% NEQ 0 goto portnotfound

:connection_check
IF NOT DEFINED NAGIOSHOST1 GOTO check_s0
IF NOT DEFINED NAGIOSHOST2 GOTO check_s1
IF NOT DEFINED NAGIOSHOST3 GOTO check_s12
GOTO check_s123

:check_s0
netstat -a -n | find "%RDP_PORT%" | find "ESTABLISHED"
IF %ERRORLEVEL% NEQ 0 goto noconnections
exit /b 2

:check_s1
netstat -a -n | find "%RDP_PORT%" | find "ESTABLISHED" | find /V "%NAGIOSHOST1%"
IF %ERRORLEVEL% NEQ 0 goto noconnections
exit /b 2

:check_s12
netstat -a -n | find "%RDP_PORT%" | find "ESTABLISHED" | find /V "%NAGIOSHOST1%" | find /V "%NAGIOSHOST2%"
IF %ERRORLEVEL% NEQ 0 goto noconnections
exit /b 2

:check_s123
netstat -a -n | find "%RDP_PORT%" | find "ESTABLISHED" | find /V "%NAGIOSHOST1%" | find /V "%NAGIOSHOST2%" | find /V "%NAGIOSHOST3%"
IF %ERRORLEVEL% NEQ 0 goto noconnections
exit /b 2

:portnotfound
echo RDP not listening! Is port %RDP_PORT% (still) correct?
exit /b 2

:usage
echo Usage: check_rdp.bat PORT HOST1 HOST2 HOST3
exit /b 3

:noconnections
echo OK: No connections.
exit /b 0
