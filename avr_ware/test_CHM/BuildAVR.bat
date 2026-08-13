avr-gcc -std=c99 -Wall -Wextra -Werror -g -Os -mmcu=avr32eb32 -o main.elf Src/main.c Src/lut.c && avr-objcopy -j .text -j .data -O ihex main.elf main.hex
pause