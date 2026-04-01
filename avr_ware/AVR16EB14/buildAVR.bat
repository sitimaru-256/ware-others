avr-gcc -g -O2 -mmcu=avr16eb14 -c -o main.o main.c && avr-gcc -g -O2 -mmcu=avr16eb14 -o main.elf main.o && avr-objcopy -j .text -j .data -O ihex main.elf main.hex && avrdude -v -p avr16eb14 -P COM5 -c serialupdi -b230400 -U flash:w:main.hex
pause
