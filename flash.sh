#!/bin/sh

FIRMWARE="FM-DX-Tuner.ino.bin"
BACKUP="backup-$(date +"%Y%m%d")-$(date +"%H%M%S").bin"
VERIFICATION="verify-$(date +"%Y%m%d")-$(date +"%H%M%S").bin"
USB_ID="0483:df11"
ADDRESS="0x08000000"
FLASH_SIZE="131072"

if ! command -v ./tef-bootloader &> /dev/null
then
    echo "ERROR: tef-bootloader not found"
    exit 1
fi

if ! command -v dfu-util &> /dev/null
then
    echo "ERROR: dfu-util not found"
    exit 1
fi

if [ $# -eq 0 ]
then
    echo "Usage: $0 port"
    echo "Example: $0 /dev/ttyACM0"
    exit 1
fi

echo '--------------------------------------------------------------------------'
echo "Switching tuner into the bootloader mode..."
echo '--------------------------------------------------------------------------'
./tef-bootloader $1
if [ $? -ne 0 ]
then
    echo "ERROR: Failed to enter bootloader mode. Trying anyway."
fi

echo '--------------------------------------------------------------------------'
echo "Creating backup of current firmware..."
echo '--------------------------------------------------------------------------'
dfu-util -w -d "$USB_ID" -a 0 -s "$ADDRESS:$FLASH_SIZE" -U "$BACKUP"
if [ $? -ne 0 ]
then
    echo "ERROR: Failed to create backup. Giving up."
    exit 1
fi

echo '--------------------------------------------------------------------------'
echo "Flashing new firmware..."
echo '--------------------------------------------------------------------------'
dfu-util -d "$USB_ID" -a 0 -s "$ADDRESS" -D "$FIRMWARE"
if [ $? -ne 0 ]
then
    echo "ERROR: Failed to update the firmware."
    exit 1
fi

echo '--------------------------------------------------------------------------'
echo "Verifying new firmware..."
echo '--------------------------------------------------------------------------'
FW_SIZE=`wc -c < "$FIRMWARE"`
dfu-util -d "$USB_ID" -a 0 -s "$ADDRESS:$FW_SIZE" -U "$VERIFICATION"
if [ $? -ne 0 ]
then
    echo "ERROR: Failed to retrieve the new firmware."
    exit 1
fi

diff "$FIRMWARE" "$VERIFICATION" &> /dev/null
if [ $? -ne 0 ]
then
    echo "ERROR: Verification FAILED, try to flash again."
    exit 1
fi

rm -f "$VERIFICATION"

echo "Update finished sucessfully."
echo '--------------------------------------------------------------------------'
echo "Starting new firmware..."
echo '--------------------------------------------------------------------------'
dfu-util -d "$USB_ID" -a 0 -s "$ADDRESS":leave

if [ $? -ne 0 ]
then
    echo "ERROR: Failed to start new firmware."
    exit 1
fi

echo '--------------------------------------------------------------------------'
echo "Firmware started sucessfully"
echo '--------------------------------------------------------------------------'

exit 0
