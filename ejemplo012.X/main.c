#define _XTAL_FREQ 8000000
#include <stdbool.h>
#include <xc.h>

unsigned short digito[] = {
    //abcdefg.
    0b11111100, //0
    0b01100000, //1
    0b11011010, //2
    0b11110010, //3
    0b01100110, //4
    0b10110110, //5
    0b10111110, //6
    0b11100000, //7
    0b11111110, //8
    0b11110110, //9
    0b11101110, //A
    0b00111110, //b
    0b10011100, //C
    0b01111010, //d
    0b10011110, //E
    0b10001110 //F
};
int numero = 86;

void printNum();

void __interrupt(high_priority) tcInt(void) {
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        TMR1H = 0x8A;
        TMR1L = 0xD0;
        //Codigo de la interrupcion
        printNum();
    }
}

void InitTimer1() {
    T1CON = 0x01;
    PIR1bits.TMR1IF = 0;
    TMR1H = 0x8A;
    TMR1L = 0xD0;
    PIE1bits.TMR1IE = 1;
    INTCON = 0xC0;
}

void main(void) {
    TRISB = 0x00;
    TRISC = 0b11111100;
    LATB = 0x00;
    LATC = 0x00;

    InitTimer1();

    for (;;) {
        numero++;
        if (numero == 100) {
            numero = 0;
        }
        __delay_ms(500);
    }
    return;
}

void printNum() {
    static bool aux = 0;
    if (!aux) { //unidades
        LATCbits.LC0 = 1;
        LATCbits.LC1 = 0;
        LATB = 255 - digito[numero % 10];
    }
    if (aux) { //decenas
        LATCbits.LC0 = 0;
        LATCbits.LC1 = 1;
        LATB = 255 - digito[numero / 10];
    }
    aux = 1 - aux;
}