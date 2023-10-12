@echo off
@echo Erase ESP flash
set /p portNumber=Enter COM port number:

esptool --chip esp32 --port COM%portNumber% erase_flash
pause