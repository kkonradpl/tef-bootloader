@echo off
for /f "tokens=2 delims==" %%a in ('wmic OS Get localdatetime /value') do set "dt=%%a"
set "YY=%dt:~2,2%" & set "YYYY=%dt:~0,4%" & set "MM=%dt:~4,2%" & set "DD=%dt:~6,2%"
set "HH=%dt:~8,2%" & set "Min=%dt:~10,2%" & set "Sec=%dt:~12,2%"

set FIRMWARE="FM-DX-Tuner.ino.bin"
set BACKUP="backup-%YYYY%%MM%%DD%-%HH%%Min%%Sec%.bin"
set VERIFICATION="verify-%YYYY%%MM%%DD%-%HH%%Min%%Sec%.bin"
set USB_ID="0483:df11"
set ADDRESS="0x08000000"
set FLASH_SIZE="131072"

echo --------------------------------------------------------------------------
echo Creating backup of current firmware...
echo --------------------------------------------------------------------------
dfu-util -w -d "%USB_ID%" -a 0 -s "%ADDRESS%:%FLASH_SIZE%" -U "%BACKUP%"
IF %ERRORLEVEL% NEQ 0 ( 
    echo ERROR: Failed to create backup. Make sure to remove existing backup.bin file.
    pause
    EXIT /B
)

echo --------------------------------------------------------------------------
echo Flashing new firmware...
echo --------------------------------------------------------------------------
dfu-util -d "%USB_ID%" -a 0 -s "%ADDRESS%" -D "%FIRMWARE%"
IF %ERRORLEVEL% NEQ 0 ( 
    echo ERROR: Failed to update the firmware.
    pause
    EXIT /B
)

echo --------------------------------------------------------------------------
echo Verifying new firmware...
echo --------------------------------------------------------------------------
FOR /F "usebackq" %%A IN ('%FIRMWARE%') DO set FW_SIZE=%%~zA
dfu-util -d "%USB_ID%" -a 0 -s "%ADDRESS%:%FW_SIZE%" -U "%VERIFICATION%"
IF %ERRORLEVEL% NEQ 0 ( 
    echo ERROR: Failed to retrieve the new firmware.
    pause
    EXIT /B
)

fc "%FIRMWARE%" "%VERIFICATION%" > nul
IF %ERRORLEVEL% NEQ 0 ( 
    echo ERROR: Verification FAILED, try to flash again.
    pause
    EXIT /B
)

del "%VERIFICATION%"

echo Update finished sucessfully.
echo --------------------------------------------------------------------------
echo Starting new firmware...
echo --------------------------------------------------------------------------

dfu-util -d "%USB_ID%" -a 0 -s "%ADDRESS%:leave"
IF %ERRORLEVEL% NEQ 0 ( 
    echo ERROR: Failed to start new firmware.
    pause
    EXIT /B
)

echo --------------------------------------------------------------------------
echo Firmware started sucessfully
echo --------------------------------------------------------------------------

pause
