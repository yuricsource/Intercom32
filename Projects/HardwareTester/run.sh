#!/bin/bash
killall gtkterm 
#make flash
#./spiffsgen.py 3080192 ./storage ./build/partitions.bin
#python /home/yuri/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/bootloader/bootloader.bin 0x10000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/app-hardware-verification.bin 0x8000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/partitions.bin

python /home/yuri/esp/esp-idf/components/esptool_py/esptool/esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 115200 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 40m --flash_size detect 0x1000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/bootloader/bootloader.bin 0x10000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/app-hardware-verification.bin 0x8000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/partitions.bin 0x110000 /home/yuri/YAR_Projects/HardwareVerificationEsp32/build/storage.bin

gtkterm --port /dev/ttyUSB0
