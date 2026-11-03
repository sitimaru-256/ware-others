#define F_CPU 20000000UL

#include <avr/io.h>
//#include <avr/cpufunc.h>
#include <util/delay.h>
#include <stdint.h>

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
uint8_t _n[4][24] = {
    {},
    {},
    {},
    {},
};
uint8_t _u[4][24] = {
    {},
    {},
    {},
    {},
};
uint8_t _m[4][24] = {
    {},
    {},
    {},
    {},
};
uint8_t _H[4][24] = {
    {},
    {},
    {},
    {},
};
void indicate1(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 177 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 0b00010001;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0b00001000;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;
        for (int col = 0; col < 20; col++){
            SPI0.DATA = figure[val][col + page*20];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
}
void indicate2(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 177 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 0b00010010;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0b00001100;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;
        for (int col = 0; col < 20; col++){
            SPI0.DATA = figure[val][col + page*20];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
}
void indicate3(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 177 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 0b00010100;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0b00000000;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;
        for (int col = 0; col < 20; col++){
            SPI0.DATA = figure[val][col + page*20];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
}
void indicate4(int val){
    for(int page = 0; page<4; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;
        SPI0.DATA = 177 + page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 0b00010101;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
        SPI0.DATA = 0b00000100;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;
        for (int col = 0; col < 20; col++){
            SPI0.DATA = figure[val][col + page*20];
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }
        PORTD.OUTSET = PIN7_bm;
    }
}

int main(void)
{
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x00);
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT4_gc;

    // PD4 = MOSI
    // PD6 = SCK
    // PD7 = CS
    PORTD.DIRSET = PIN4_bm | PIN6_bm | PIN7_bm;

    // PC0 = RS
    // PC1 = RESET
    PORTC.DIRSET = PIN0_bm | PIN1_bm;

    PORTD.OUTSET = PIN7_bm;
    PORTC.OUTSET = PIN1_bm;

    // --------------------------------
    // 3. SPI設定
    // --------------------------------
    SPI0.CTRLB = SPI_MODE_3_gc;

    SPI0.CTRLA =
        SPI_ENABLE_bm |
        SPI_MASTER_bm |
        SPI_PRESC_DIV4_gc |
        SPI_CLK2X_bm;

    // --------------------------------
    // 4. LCDリセット
    // --------------------------------
    PORTC.OUTCLR = PIN1_bm;
    _delay_ms(10);

    PORTC.OUTSET = PIN1_bm;
    _delay_ms(10);

    // --------------------------------
    // 5. LCD初期化
    // --------------------------------
    PORTD.OUTCLR = PIN7_bm;
    PORTC.OUTCLR = PIN0_bm;

    // Display OFF
    SPI0.DATA = 0xAE;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // ADC select
    SPI0.DATA = 0xA0;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // COM scan direction
    SPI0.DATA = 0xC8;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Bias
    SPI0.DATA = 0xA3;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Power control
    SPI0.DATA = 0x2C;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    _delay_ms(2);

    SPI0.DATA = 0x2E;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));
    _delay_ms(2);

    SPI0.DATA = 0x2F;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Resistor ratio
    SPI0.DATA = 0x23;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Electronic volume
    SPI0.DATA = 0x81;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    SPI0.DATA = 0x1C;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Display normal
    SPI0.DATA = 0xA4;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Start line
    SPI0.DATA = 0x40;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    // Display ON
    SPI0.DATA = 0xAF;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    PORTD.OUTSET = PIN7_bm;

    // 6. 画面を全消去
    for (uint8_t page = 0; page < 6; page++){
        PORTD.OUTCLR = PIN7_bm;
        PORTC.OUTCLR = PIN0_bm;

        SPI0.DATA = 0xB0 | page;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 0x10;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        SPI0.DATA = 0x00;
        while (!(SPI0.INTFLAGS & SPI_IF_bm));

        PORTC.OUTSET = PIN0_bm;

        for (uint8_t col = 0; col < 128; col++)
        {
            SPI0.DATA = 0x00;
            while (!(SPI0.INTFLAGS & SPI_IF_bm));
        }

        PORTD.OUTSET = PIN7_bm;
    }

/*
    PORTD.OUTCLR = PIN7_bm;
    PORTC.OUTCLR = PIN0_bm;


    SPI0.DATA = 0xB1;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));


    SPI0.DATA = 0x10;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));

    SPI0.DATA = 0x00;
    while (!(SPI0.INTFLAGS & SPI_IF_bm));


    PORTC.OUTSET = PIN0_bm;

    for (uint8_t col = 0; col < 23; col++)
    {
        SPI0.DATA = _H[0][col];
        while (!(SPI0.INTFLAGS & SPI_IF_bm));
    }

    PORTD.OUTSET = PIN7_bm;
    */
    
    indicate1(2);
    indicate2(3);
    indicate3(1);
    indicate4(4);
    int innr = 0;

    while (1){
        innr +=7;
        indicate1(innr/1000);
        indicate2((innr/100)%10);
        indicate3((innr/10)%10);
        indicate4(innr%10);
        _delay_ms(200);
    }
}
