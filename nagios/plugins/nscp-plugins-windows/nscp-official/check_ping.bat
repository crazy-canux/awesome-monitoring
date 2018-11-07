@echo off
ping -n 1 %1 -w 20000 >NUL
IF ERRORLEVEL 2 GOTO unknown
IF ERRORLEVEL 1 GOTO err
GOTO ok
 
:err
echo CRITICAL: Ping check failed
exit /B 1
 
:unknown
echo UNKNOWN: Something went wrong
exit /B 3
 
:ok
echo OK: Ping succeded
exit /B 0