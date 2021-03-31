@ECHO OFF

arm-none-eabi-gcc -c -mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wall -Wno-main -Werror -ffunction-sections -O2 -fPIC -I. %1.c -o %1.o
IF ERRORLEVEL 1 exit 1

arm-none-eabi-ld -nostartfiles -T arm-gcc-link.ld -o %1.elf %1.o
IF ERRORLEVEL 1 exit 1

MMBasic ArmCfGenV143%2.bas %1.elf
IF ERRORLEVEL 1 exit 1

:Done
