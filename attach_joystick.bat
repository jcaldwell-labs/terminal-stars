@echo off
REM Joystick USB Passthrough Helper for WSL2
REM Run this in Windows PowerShell as Administrator

echo ============================================
echo  Joystick USB Passthrough for WSL2
echo ============================================
echo.

echo Step 1: Listing USB devices...
echo.
usbipd list
echo.

echo Step 2: Find your joystick in the list above
echo Look for: Xbox, PlayStation, Logitech, or other game controller
echo.
set /p BUSID="Enter the BUSID (e.g., 1-4): "

echo.
echo Step 3: Binding device %BUSID%...
usbipd bind --busid %BUSID%

echo.
echo Step 4: Attaching to WSL...
usbipd attach --wsl --busid %BUSID%

echo.
echo ============================================
echo  Done! Joystick should now be available in WSL2
echo ============================================
echo.
echo To verify in WSL2, run:
echo   ./check_joystick.sh
echo.
echo To detach the device:
echo   usbipd detach --busid %BUSID%
echo.

pause
