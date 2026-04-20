#include <avr/io.h>
#include <avr/interrupt.h>
volatile float frq;
volatile float basfrq = 10.0;
float offset;
float ratio;
float dutyu;
float dutyv;
float dutyw;
int i = 0;
float sin27u[] = {0.995,0.985,0.97,0.945,0.92,0.885,0.845,0.8,0.75,0.7,0.645,0.585,0.53,0.47,0.415,0.355,0.3,0.25,0.2,0.155,0.115,0.08,0.055,0.03,0.015,0.005,0.0,0.005,0.015,0.03,0.055,0.08,0.115,0.155,0.2,0.25,0.3,0.355,0.415,0.47,0.53,0.585,0.645,0.7,0.75,0.8,0.845,0.885,0.92,0.945,0.97,0.985,0.995,1.0};
float sin27v[] = {0.2,0.155,0.115,0.08,0.055,0.03,0.015,0.005,0.0,0.005,0.015,0.03,0.055,0.08,0.115,0.155,0.2,0.25,0.3,0.355,0.415,0.47,0.53,0.585,0.645,0.7,0.75,0.8,0.845,0.885,0.92,0.945,0.97,0.985,0.995,1.0,0.995,0.985,0.97,0.945,0.92,0.885,0.845,0.8,0.75,0.7,0.645,0.585,0.53,0.47,0.415,0.355,0.3,0.25};
float sin27w[] = {0.3,0.355,0.415,0.47,0.53,0.585,0.645,0.7,0.75,0.8,0.845,0.885,0.92,0.945,0.97,0.985,0.995,1.0,0.995,0.985,0.97,0.945,0.92,0.885,0.845,0.8,0.75,0.7,0.645,0.585,0.53,0.47,0.415,0.355,0.3,0.25,0.2,0.155,0.115,0.08,0.055,0.03,0.015,0.005,0.0,0.005,0.015,0.03,0.055,0.08,0.115,0.155,0.2,0.25};

ISR(TCB0_INT_vect){
	TCB0.INTFLAGS = 1;
	basfrq = basfrq + 0.0005;
}

ISR(TCB1_INT_vect){
	TCB1.INTFLAGS = 1;
	basfrq = basfrq + 0.0005;
}

ISR(TCE0_OVF_vect){
	TCE0.INTFLAGS = 1;
	frq = basfrq * 27.0;
	ratio = basfrq * 0.0168;
    offset = 0.5 - basfrq * 0.0084;
	dutyu = sin27u[i] * ratio + offset;
    dutyv = sin27v[i] * ratio + offset;
    dutyw = sin27w[i] * ratio + offset;
	TCE0.PER = (unsigned int)(2500000 / frq);
	TCE0.CMP0 = (unsigned int)(2500000 / frq * dutyu);
	TCE0.CMP1 = (unsigned int)(2500000 / frq * dutyv);
	TCE0.CMP2 = (unsigned int)(2500000 / frq * dutyw);
	i++;
	if(i>26){i = 0;}
}

void clock_init(void){
	CPU_CCP = 0xD8; //IOREG write protected register
	CLKCTRL.MCLKCTRLB = 0x0; //no prescaling
}
void tce_init(void){
	TCE0.PER = (unsigned int)(2500000 / frq);
	TCE0.CTRLA = 0b00000101; //div4 timer enable
	TCE0.CTRLB = 0b01110110;
	TCE0.INTCTRL = TCE_OVF_bm;
}

int main(void){
	clock_init();
	tce_init();

	TCB0.CCMP = 5000;
	TCB0.CNT = 0x0;
	TCB0.CTRLB = TCB_CNTMODE_INT_gc;
	TCB0.EVCTRL = 0x0;
	TCB0.INTCTRL = TCB_CAPT_bm;
	TCB0.CTRLA = TCB_ENABLE_bm;

	TCB1.CCMP = 5000;
	TCB1.CNT = 0x0;
	TCB1.CTRLB = TCB_CNTMODE_INT_gc;
	TCB1.EVCTRL = 0x0;
	TCB1.INTCTRL = TCB_CAPT_bm;
	TCB1.CTRLA = TCB_ENABLE_bm;

	PORTMUX.TCEROUTEA = 0x2;
	PORTC.DIRSET = 0b00000111;
	sei();
	while (1) {}
}
