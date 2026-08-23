#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
#include<stdlib.h>
#include"lut.h"

uint32_t alpha_sort[3][90];
uint32_t alpha_est[3][90];
uint16_t alpha_cur[3][90];
int alpha_num = 7;
int alpha_num_cur = 7;
float ratio = 0.0;
float AMP;
float OFFSET;
float dutyu;
float dutyv;
float dutyw;
float dutyun;
float dutyvn;
float dutywn;
float Asdutyu;
float Asdutyv;
float Asdutyw;
int dir;
int motorState = 0;
int ampINT = 0;
int transit = 1;
float frq;
float basfrq = 1;
float basfrq_Jerk = 0.0;
int JerkPole;
int tca_cnt = 0;
int tcb_cnt = 0;
int tcb2_cnt = 0;
int pulse_mode;
int pnum_PWM;
float PER;
float PER_PWM;

float min(float a, float b){
    return (a < b) ? a : b;
}
float max(float a, float b) {
    return (a > b) ? a : b;
}

void swap (uint32_t *x, uint32_t *y){
	uint32_t temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void shell_sort (void){
	int i, j, h, array_size;
	array_size = alpha_num * 12 + 6;
	for(h = 1; h <= array_size/9; h = 3*h + 1);
	for( ; h > 0; h /= 3){
		for (i = h; i < array_size; i++){
			j = i;
			while((j > h - 1) && (alpha_sort[0][j-h] > alpha_sort[0][j])) {
				swap(&alpha_sort[0][j-h], &alpha_sort[0][j]);
				swap(&alpha_sort[1][j-h], &alpha_sort[1][j]);
				swap(&alpha_sort[2][j-h], &alpha_sort[2][j]);
				j -= h;
			}
		}
	}
}

void makePER(const int alpha[][alpha_num], const int pole[], int amp){
    int array_cnt = 0;
	int array_cnt_sus = 0;
	PER = 2500000 / basfrq;
	while(array_cnt < alpha_num){
		alpha_sort[0][array_cnt_sus] = (uint32_t)((pgm_read_word(&alpha[amp][array_cnt])) * PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((12000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 2;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((24000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 4;
		array_cnt++;
		array_cnt_sus++;
	}
	while(array_cnt > 0){
		array_cnt--;
		alpha_sort[0][array_cnt_sus] = (uint32_t)((18000 - pgm_read_word(&alpha[amp][array_cnt])) * PER / 36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((30000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 2;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((42000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 4;
		array_cnt_sus++;
	}
	while(array_cnt < alpha_num){
		alpha_sort[0][array_cnt_sus] = (uint32_t)((18000 + pgm_read_word(&alpha[amp][array_cnt])) * PER / 36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((30000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 2;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((42000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 4;
		array_cnt++;
		array_cnt_sus++;
	}
	while(array_cnt > 0){
		array_cnt--;
		alpha_sort[0][array_cnt_sus] = (uint32_t)((36000 - pgm_read_word(&alpha[amp][array_cnt])) * PER / 36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((48000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 2;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((60000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 4;
		array_cnt_sus++;
	}
	alpha_sort[2][array_cnt_sus+alpha_num*8] = 1;
	alpha_sort[2][array_cnt_sus+alpha_num*8+3] = 1;
	alpha_sort[2][array_cnt_sus+alpha_num*8+1] = 2;
	alpha_sort[2][array_cnt_sus+alpha_num*8+4] = 2;
	alpha_sort[2][array_cnt_sus+alpha_num*8+2] = 4;
	alpha_sort[2][array_cnt_sus+alpha_num*8+5] = 4;
	for(int i=0; i<3; i++){
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)(i * PER / 3) % (uint32_t)PER;
		alpha_sort[0][array_cnt_sus+alpha_num*8+3] = (uint32_t)((3 + i*2) * PER / 6) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+1)%2;
		alpha_sort[1][array_cnt_sus+alpha_num*8+3] = (pgm_read_byte(&pole[amp])+2)%2;
		array_cnt_sus++;
	}
	array_cnt = 0;
	shell_sort();
	for(int i = 0; i < alpha_num*12+6 - 1; i++){
		alpha_sort[0][i] = alpha_sort[0][i+1] - alpha_sort[0][i];
		if(alpha_sort[0][i] < 100){alpha_sort[0][i] = 100;}
	}
	alpha_sort[0][alpha_num*12+6 - 1] = (uint32_t)PER - alpha_sort[0][alpha_num*12+6 - 1];
	if(alpha_sort[0][alpha_num*12+6 - 1] < 100){alpha_sort[0][alpha_num*12+6 - 1] = 100;}
	for(int i = 0; i < alpha_num*12+6; i++){
		alpha_est[0][i] = alpha_sort[0][i]*4;
		alpha_est[1][i] = alpha_sort[1][i];
		alpha_est[2][i] = alpha_sort[2][i];
	}
}

enum pulse_type{
	Async,
	SyncMp,
	S3p,
	W3p,
	CHM,
	SHE,
	HO
};

ISR(TCA0_OVF_vect){
	TCA0.SINGLE.INTFLAGS = 0x1;
	if(pulse_mode == 0){
		PER_PWM = 2500000 / frq;
		dutyu = Asdutyu * AMP / 32767 + OFFSET;
		dutyv = Asdutyv * AMP / 32767 + OFFSET;
		dutyw = Asdutyw * AMP / 32767 + OFFSET;
		dutyun = Asdutyu * AMP / 32767 + OFFSET;
		dutyvn = Asdutyv * AMP / 32767 + OFFSET;
		dutywn = Asdutyw * AMP / 32767 + OFFSET;
	}
	else if(pulse_mode == 1){
		PER_PWM = 2500000 / (basfrq*pnum_PWM);
	}
	TCA0.SINGLE.PER = (uint16_t)PER_PWM;
	TCA0.SINGLE.PERBUF = (uint16_t)PER_PWM;
    TCA0.SINGLE.CMP0 = (uint16_t)PER_PWM*dutyu;
    TCA0.SINGLE.CMP1 = (uint16_t)PER_PWM*dutyv;
    TCA0.SINGLE.CMP2 = (uint16_t)PER_PWM*dutyw;
	TCA0.SINGLE.CMP0BUF = (uint16_t)PER_PWM*dutyun;
    TCA0.SINGLE.CMP1BUF = (uint16_t)PER_PWM*dutyvn;
    TCA0.SINGLE.CMP2BUF = (uint16_t)PER_PWM*dutywn;
}

ISR(TCB0_INT_vect){
	TCB0.INTFLAGS = 0x1;
	if(tcb_cnt == 0){
		alpha_num_cur = alpha_num;
		for(int i = 0; i < alpha_num_cur*12+6; i++){
			alpha_cur[0][i] = (uint16_t)(alpha_est[0][i]);
			alpha_cur[1][i] = (uint16_t)(alpha_est[1][i]);
			alpha_cur[2][i] = (uint16_t)(alpha_est[2][i]);
		}
		PORTC.OUTSET = 0b00000010;
		PORTC.OUTCLR = 0b00000100;
	}
	if(alpha_cur[1][tcb_cnt] == 1){
		PORTC.OUTSET = alpha_cur[2][tcb_cnt];
	}
	else if(alpha_cur[1][tcb_cnt] == 0){
		PORTC.OUTCLR = alpha_cur[2][tcb_cnt];
	}
	TCB0.CCMP = alpha_cur[0][tcb_cnt];
	tcb_cnt += 1;
	if(tcb_cnt > alpha_num_cur*12+6 - 1){tcb_cnt = 0;}
}
ISR(TCB1_INT_vect){
	//加速と変調率の計算
	TCB1.INTFLAGS = 0x1;
	basfrq += (0.0004 * dir);
	if(JerkPole != 0){basfrq_Jerk += (0.008*JerkPole);}
}
ISR(TCB2_INT_vect){
	TCB2.INTFLAGS = 0x1;
	if(tcb2_cnt == 0){
		if(pulse_mode != 0 && pulse_mode != 1){
			if(transit == 0){
				TCA0.SINGLE.CTRLB = 0b00000101;transit = 1;
				tcb_cnt = 0;
			}
		}
		else{
			if(transit == 1){TCA0.SINGLE.CTRLB = 0b01110101;transit = 0;}
		}
	}
	TCB2.CCMP = (uint16_t)(151515.151515 / basfrq);
	if(pulse_mode == 0 || transit == 0){
		Asdutyu = pgm_read_word(&SYNC[0][tcb2_cnt%66]);
		Asdutyv = pgm_read_word(&SYNC[0][(tcb2_cnt+22)%66]);
		Asdutyw = pgm_read_word(&SYNC[0][(tcb2_cnt+44)%66]);
	}
	tcb2_cnt++;
	tcb2_cnt %= 66;
}

int main(void){
    makePER(_7alpha, _7alpha_pole, ampINT);

	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x0);

    TCB0.CCMP = 0x9C4;
	TCB0.CNT = 0x0;
	TCB0.CTRLB = 0b01000000;
	TCB0.EVCTRL = 0x0;
	TCB0.INTCTRL = 0x1;
	TCB0.CTRLA = 0x3;

	TCB1.CCMP = 0x9C4;
	TCB1.CNT = 0x0;
	TCB1.CTRLB = 0b01000000;
	TCB1.EVCTRL = 0x0;
	TCB1.INTCTRL = 0x1;
	TCB1.CTRLA = 0x1;

	TCB2.CCMP = 0x9C4;
	TCB2.CNT = 0x0;
	TCB2.CTRLB = 0x0;
	TCB2.EVCTRL = 0x0;
	TCB2.INTCTRL = 0x1;
	TCB2.CTRLA = 0x3;

	TCA0.SINGLE.PER = 0xFFFF;
	TCA0.SINGLE.CTRLA = 0b00000101; //div4 timer enable
	TCA0.SINGLE.CTRLB = 0b00000101;
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm;

	PORTMUX.TCAROUTEA = 0b00000010;

	PORTC.DIRSET = 0b00000111;
	PORTD.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTD.PIN5CTRL = PORT_PULLUPEN_bm;
    sei();
    while(1){
		if (~PORTD.IN & PIN4_bm){
			motorState = 1;
			if(basfrq_Jerk < basfrq){
				JerkPole = 1;
				ratio = min(max(basfrq_Jerk * 8.35, 5), 501);
				AMP = min(max(basfrq_Jerk * 0.0185, 0), 1);
                OFFSET = min(max(0.5 - basfrq_Jerk * 0.00925, 0), 0.5);
			}
			else{
				dir = 1;
				JerkPole = 0;
				basfrq_Jerk = basfrq + 1;
				ratio = min(max(basfrq * 8.35, 5), 501);
				AMP = min(max(basfrq * 0.0185, 0), 1);
                OFFSET = min(max(0.5 - basfrq * 0.00925, 0), 0.5);
			}
		}
		else if (~PORTD.IN & PIN5_bm){
			motorState = -1;
			if(basfrq_Jerk < basfrq){
				JerkPole = 1;
				ratio = min(max(basfrq_Jerk * 6.863, 5), 501);
				AMP = min(max(basfrq_Jerk * 0.0103, 0), 1);
                OFFSET = min(max(0.5 - basfrq_Jerk * 0.00515, 0), 0.5);
			}
			else{
				dir = -1;
				JerkPole = 0;
				basfrq_Jerk = basfrq;
				ratio = min(max(basfrq * 6.863, 5), 501);
				AMP = min(max(basfrq * 0.0103, 0), 1);
                OFFSET = min(max(0.5 - basfrq * 0.00515, 0), 0.5);
			}
		}
		else{
			dir = 0;
			if(basfrq_Jerk > 0){
				JerkPole = -1;
				ratio = min(max(basfrq_Jerk * 6.863, 5), 501);
				AMP = min(max(basfrq_Jerk * 0.0103, 0), 1);
                OFFSET = min(max(0.5 - basfrq_Jerk * 0.00515, 0), 0.5);
			}
			else{
				JerkPole = 0;
				basfrq_Jerk = 0;
				motorState = 0;
			}
		}

		if(motorState == 1){
			if(basfrq >= 80 && dir == 1){alpha_num = 0;}
			else if(basfrq >= 59){pulse_mode = CHM;alpha_num = 1;}
			else if(basfrq >= 57){pulse_mode = CHM;alpha_num = 2;}
			else if(basfrq >= 43.5){pulse_mode = CHM;alpha_num = 3;}
			else if(basfrq >= 37){pulse_mode = CHM;alpha_num = 4;}
			else if(basfrq >= 30){pulse_mode = CHM;alpha_num = 5;}
			else if(basfrq >= 27){pulse_mode = CHM;alpha_num = 6;}
    		else if(basfrq >= 24){pulse_mode = CHM;alpha_num = 7;}
			else if(basfrq >= 0){pulse_mode = Async;frq = 400;}
		}
		else if(motorState == -1){
			if(basfrq >= 80 && dir == -1){pulse_mode = CHM;alpha_num = 0;}
			else if(basfrq >= 70.7){pulse_mode = CHM;alpha_num = 1;}
			else if(basfrq >= 63){pulse_mode = CHM;alpha_num = 2;}
			else if(basfrq >= 41){pulse_mode = CHM;alpha_num = 3;}
			else if(basfrq >= 34.5){pulse_mode = CHM;alpha_num = 4;}
			else if(basfrq >= 29){pulse_mode = CHM;alpha_num = 5;}
			else if(basfrq >= 25){pulse_mode = CHM;alpha_num = 6;}
			else if(basfrq >= 22.5){pulse_mode = CHM;alpha_num = 7;}
			else if(basfrq >= 0){pulse_mode = Async;frq = 400;}
		}

		if(alpha_num == 7 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_7alpha, _7alpha_pole, ampINT);}
		else if(alpha_num == 6 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_6alpha, _6alpha_pole, ampINT);}
		else if(alpha_num == 5 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_5alpha, _5alpha_pole, ampINT);}
		else if(alpha_num == 4 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_4alpha, _4alpha_pole, ampINT);}
		else if(alpha_num == 3 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_3alpha, _3alpha_pole, ampINT);}
		else if(alpha_num == 2 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_2alpha, _2alpha_pole, ampINT);}
		else if(alpha_num == 1 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_1alpha, _1alpha_pole, ampINT);}
		else if(alpha_num == 0 && pulse_mode == 4){ampINT = (uint16_t)ratio; makePER(_1alpha, _1alpha_pole, 100);}
	}
}
