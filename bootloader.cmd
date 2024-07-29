@echo off
set PORT="COM4"

echo --------------------------------------------------------------------------
echo Switching tuner into the bootloader mode...
echo --------------------------------------------------------------------------
tef-bootloader %PORT%
pause
