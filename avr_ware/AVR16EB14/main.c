#define F_CPU 20000000UL

#include <avr/io.h>
#include <util/delay.h>
float frq = 1000.0;
float duty = 0.5;

void clock_init(void) {
}

int main(void)
{
	clock_init();
	TCE0.PER = (unsigned int)(2000000 / frq);
	TCE0.CTRLA = 0b00000001;
	TCE0.CTRLB = 0b00010110;
	PORTA.DIRSET = 0b00000001;
	PORTMUX. TCEROUTEA = 0b00000000;
	while (1) {
		_delay_ms(2);
		duty += 0.01;
		if(duty > 1.0){duty = 0;}
		TCE0.CMP0 = (unsigned int)(2000000 / frq * duty);
    }
}
