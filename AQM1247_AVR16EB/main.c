#define F_CPU 20000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
int column = 0;
uint32_t cnt_est = 0;
uint32_t cnt_cur = 0;
uint32_t cnt_pre = 0;

uint8_t figure[10][80] = {
    {
        0, 0, 0, 192, 248, 252, 252, 62, 30, 30, 30, 30, 62, 252, 252, 248, 224, 0, 0, 0,
        0, 240, 255, 255, 255, 7, 0, 0, 0, 0, 0, 0, 0, 0, 7, 255, 255, 255, 248, 0,
        0, 31, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 192, 255, 255, 255, 63, 0,
        0, 0, 1, 7, 63, 127, 126, 248, 240, 240, 240, 240, 248, 126, 127, 63, 15, 1, 0, 0
    },
    {
        0, 0, 0, 128, 192, 192, 224, 240, 252, 254, 254, 254, 254, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 3, 7, 3, 3, 1, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0
    },
    {
        0, 0, 96, 240, 252, 124, 62, 62, 30, 30, 30, 30, 126, 254, 252, 248, 224, 128, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 240, 255, 255, 63, 15, 0, 0,
        0, 0, 0, 0, 0, 0, 128, 192, 240, 248, 252, 126, 31, 15, 7, 1, 0, 0, 0, 0,
        0, 0, 240, 248, 254, 255, 255, 255, 241, 241, 240, 240, 240, 240, 240, 240, 240, 240, 0, 0
    },
    {
        0, 0, 16, 120, 124, 62, 62, 30, 30, 30, 30, 30, 62, 254, 252, 252, 240, 0, 0, 0, 0,
        0, 0, 0, 0, 192, 192, 192, 192, 192, 192, 224, 240, 125, 127, 63, 15, 0, 0, 0, 0,
        0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 7, 15, 255, 254, 252, 240, 0, 0,
        0, 0, 60, 124, 248, 248, 240, 240, 240, 240, 240, 240, 248, 252, 127, 127, 31, 7, 0, 0
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 192, 224, 248, 252, 254, 254, 254, 254, 0, 0, 0, 0,
        0, 0, 0, 0, 224, 240, 248, 254, 31, 7, 3, 0, 255, 255, 255, 255, 0, 0,
        0, 0, 224, 248, 252, 255, 239, 227, 225, 224, 224, 224, 224, 224, 255, 255, 255, 255, 224, 224,
        224, 224, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 255, 255, 255, 255, 1, 1, 1, 1
    },
    {
        0, 0, 0, 254, 254, 254, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0,
        0, 0, 0, 255, 255, 255, 224, 224, 224, 224, 224, 224, 192, 192, 128, 128, 0, 0, 0, 0,
        0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 3, 7, 255, 255, 255, 252, 0, 0,
        0, 0, 60, 124, 124, 248, 240, 240, 240, 240, 240, 240, 248, 252, 127, 63, 63, 7, 0, 0, 0
    },
    {
        0, 0, 0, 128, 224, 240, 248, 252, 62, 62, 30, 30, 30, 30, 30, 30, 30, 0, 0,
        0, 0, 192, 252, 255, 255, 159, 195, 192, 224, 240, 240, 240, 240, 240, 224, 224, 128, 0, 0,
        0, 0, 31, 255, 255, 255, 231, 3, 1, 0, 0, 0, 0, 1, 1, 7, 255, 255, 255, 252, 0, 0,
        0, 1, 7, 63, 127, 126, 248, 240, 240, 240, 240, 240, 248, 126, 127, 31, 15, 3, 0
    },
    {
        0, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 158, 222, 254, 254, 254, 62, 30, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 128, 192, 240, 252, 255, 63, 15, 3, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 128, 240, 255, 255, 255, 31, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 248, 255, 255, 255, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    {
        0, 0, 192, 240, 252, 252, 126, 30, 30, 30, 30, 30, 30, 126, 252, 252, 240, 192, 0, 0,
        0, 0, 7, 31, 127, 255, 248, 240, 224, 192, 192, 192, 240, 248, 127, 127, 31, 7, 0,
        0, 0, 192, 240, 248, 254, 62, 15, 7, 3, 3, 3, 7, 15, 31, 63, 254, 252, 248, 224, 0,
        0, 7, 31, 63, 127, 252, 248, 240, 240, 240, 240, 240, 240, 248, 252, 127, 63, 31, 7, 0
    },
    {
        0, 128, 224, 240, 252, 252, 62, 62, 30, 30, 30, 30, 62, 124, 252, 248, 224, 128, 0,
        0, 0, 127, 255, 255, 255, 192, 0, 0, 0, 0, 0, 0, 0, 0, 131, 255, 255, 255, 248, 0, 0,
        0, 1, 3, 15, 15, 15, 31, 30, 30, 30, 30, 15, 135, 247, 255, 255, 127, 7, 0, 0,
        0, 0, 240, 240, 240, 240, 240, 240, 240, 248, 248, 126, 63, 31, 15, 3, 0, 0, 0
    }
};
//unit: 0=nH, 1=uH, 2=mH, 3=H
uint8_t unit[4][160] = {
    {0, 0, 0, 128, 128, 128, 0, 128, 128, 128, 128, 128, 0, 0, 0, 0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 3, 3, 3, 3, 3, 255, 255, 254, 0, 0, 0, 255, 255, 255, 28, 28, 28, 28, 28, 28, 28, 28, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 0, 0, 0, 192, 192, 0, 0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 248, 255, 31, 3, 0, 0, 0, 128, 240, 255, 63, 1, 0, 0, 0, 255, 255, 255, 28, 28, 28, 28, 28, 28, 28, 28, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 240, 254, 63, 15, 28, 28, 28, 28, 30, 15, 15, 31, 28, 24, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 128, 128, 128, 0, 0, 128, 128, 128, 128, 0, 0, 0, 0, 128, 128, 128, 128, 0, 0, 0, 0, 0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 252, 252, 252, 0, 0, 0, 255, 255, 255, 3, 3, 3, 3, 3, 255, 255, 255, 7, 3, 3, 3, 3, 255, 255, 254, 0, 0, 0, 0, 255, 255, 255, 28, 28, 28, 28, 28, 28, 28, 28, 255, 255, 255, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 252, 252, 252, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 28, 28, 28, 28, 28, 28, 28, 28, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
uint8_t dot[8]={0,0,60,60,60,60,0,0};

void icf(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 0xB1 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 16 + ((column >> 4) & 0x0F);
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = column & 0x0F;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;
        for (int col = 0; col < 20; col++){
            SPI0.DATA = figure[val][col + page*20];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
    column += 20;
}
void icf_end(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 177 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 16 + ((column >> 4) & 0x0F);
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = column & 0x0F;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;
        for (int col = 0; col < 20; col++){
            SPI0.DATA = figure[val][col + page*20];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
    column = 0;
}
void icdot(void){
    PORTD.OUTCLR = PIN7_bm;
    PORTC.OUTCLR = PIN0_bm;
    SPI0.DATA = 0xB4;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    SPI0.DATA = 16 + ((column >> 4) & 0x0F);
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = column & 0x0F;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    PORTC.OUTSET = PIN0_bm;
    for (int col = 0; col < 8; col++){
        SPI0.DATA = dot[col];
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
    }
    PORTD.OUTSET = PIN7_bm;
    column += 8;
}
void icUnit(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;

        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 0xB2 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0b00010101;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0b00001000;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        PORTC.OUTSET = PIN0_bm;

        for (int col = 0; col < 40; col++){
            SPI0.DATA = unit[val][col + page*40];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
}
void init_LCD(void){
    SPI0.CTRLB = SPI_MODE_3_gc;
    SPI0.CTRLA = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_PRESC_DIV4_gc | SPI_CLK2X_bm;

    PORTC.OUTCLR = PIN1_bm;
    _delay_ms(10);
    PORTC.OUTSET = PIN1_bm;
    _delay_ms(10);

    PORTD.OUTCLR = PIN7_bm;
    PORTC.OUTCLR = PIN0_bm;
    SPI0.DATA = 0xAE;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0xA0;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0xC8;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0xA3;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0x2C;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    _delay_ms(2);
    SPI0.DATA = 0x2E;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    _delay_ms(2);
    SPI0.DATA = 0x2F;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0x23;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0x81;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0x1C;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0xA4;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0x40;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    SPI0.DATA = 0xAF;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    PORTC.OUTSET = PIN0_bm;
    PORTD.OUTSET = PIN7_bm;

    for (uint8_t page = 0; page < 6; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 0xB0 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0x10;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0x00;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        PORTC.OUTSET = PIN0_bm;
        for (uint8_t col = 0; col < 128; col++){
            SPI0.DATA = 0x00;
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
}
void init_EVS(void){
    EVSYS.CHANNEL0 = EVSYS_CHANNEL_PORTA_EV0_gc;
    EVSYS.USERTCB0COUNT = EVSYS_USER_CHANNEL0_gc;
    EVSYS.CHANNEL1 = EVSYS_CHANNEL_TCB0_OVF_gc;
    EVSYS.USERTCB1COUNT = EVSYS_USER_CHANNEL1_gc;
    EVSYS.CHANNEL2 = EVSYS_CHANNEL_RTC_OVF_gc;
    EVSYS.USERTCB0CAPT = EVSYS_USER_CHANNEL2_gc;
    EVSYS.USERTCB1CAPT = EVSYS_USER_CHANNEL2_gc;
}
void init_TCB(void){
    TCB0.CTRLB = TCB_ASYNC_bm | TCB_CNTMODE_1_bm | TCB_CNTMODE_0_bm;
    TCB1.CTRLB = TCB_ASYNC_bm | TCB_CNTMODE_1_bm | TCB_CNTMODE_0_bm;
    TCB0.EVCTRL = TCB_CAPTEI_bm;
    TCB1.EVCTRL = TCB_CAPTEI_bm;
    TCB0.CNT = 0;
    TCB1.CNT = 0;
    TCB1.CTRLA = TCB_CASCADE_bm | TCB_CLKSEL_EVENT_gc | TCB_ENABLE_bm;
    TCB0.CTRLA = TCB_CLKSEL_EVENT_gc | TCB_ENABLE_bm;
}
void init_RTC(void){
    RTC.PER = 0x1FFF;
    RTC.INTCTRL = RTC_OVF_bm;
    RTC.CTRLA = RTC_RTCEN_bm;
}
void init_AC(void){
    AC0.MUXCTRL = AC_MUXPOS_AINP6_gc | AC_MUXNEG_AINN3_gc;
    AC0.CTRLA = AC_HYSMODE_gm | AC_ENABLE_bm;
}
void init_CCL(void){
    CCL.LUT1CTRLB = CCL_INSEL0_AC0_gc;
    CCL.TRUTH1 = 0xAA;
    CCL.LUT1CTRLA = CCL_ENABLE_bm | CCL_OUTEN_bm | CCL_FILTSEL_DISABLE_gc;
    CCL.CTRLA = CCL_ENABLE_bm;
}

ISR(RTC_CNT_vect){
    RTC.INTFLAGS = RTC_OVF_bm;
    cnt_cur = ((uint32_t)TCB1.CCMP << 16) | (uint32_t)TCB0.CCMP;
    icf((cnt_cur*4/10000)%10);
    icf((cnt_cur*4/1000)%10);
    icf((cnt_cur*4/100)%10);
    icf((cnt_cur*4/10)%10);
    icf_end(cnt_cur*4%10);
}

int main(void){
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT4_gc;
    PORTD.DIRSET = PIN4_bm | PIN6_bm | PIN7_bm; // PD4=SDA, PD6=SCL, PD7=CS
    PORTC.DIRSET = PIN0_bm | PIN1_bm; //PC0=RS, PC1=RESET
    PORTD.OUTSET = PIN7_bm;
    PORTC.OUTSET = PIN1_bm;

    PORTA.DIRCLR = PIN1_bm;
    PORTA.PIN1CTRL = PORT_ISC_INTDISABLE_gc;
    PORTA.EVGENCTRLA = PORT_EVGEN0SEL_PIN1_gc;

    PORTC.DIRCLR = PIN2_bm; // PC2=in-
    PORTD.DIRCLR = PIN5_bm; // PD5=in+
    PORTC.DIRSET = PIN3_bm; // PC3=Vcomp
    PORTC.PIN2CTRL = PORT_ISC_INPUT_DISABLE_gc;
    PORTD.PIN5CTRL = PORT_ISC_INPUT_DISABLE_gc;

    init_EVS();
    init_TCB();
    init_RTC();
    init_AC();
    init_CCL();
    init_LCD();
    
    sei();

    while (1){
        //icf((cnt_cur*4/10000000)%10);
    //icf((cnt_cur*4/1000000)%10);
    //icf((cnt_cur*4/100000)%10);
    //icf((cnt_cur*4/10000)%10);
    //icf_end(cnt_cur*4/1000%10);
    }
}
