avr-gcc -Wall -Wextra -Werror -g -Os -mmcu=avr32eb32 -o main.elf Src/main.c Src/lut.c && avr-objcopy -j .text -j .data -O ihex main.elf main.hex && avrdude -v -p avr32eb32 -P COM5 -c serialupdi -b230400 -U flash:w:main.hex
pause
