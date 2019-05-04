#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

const uint8_t digitos[16] = {
    //.gfedcba
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111, //9
    0b01110111, //A
    0b01111100, //b
    0b00111001, //C
    0b01011110, //d
    0b01111001, //E
    0b01110001, //F
};

void mostrar(uint8_t conta) {
    uint8_t unidades = conta % 10;
    uint8_t decenas = conta / 10;
    //unidades
    PORTAbits.RA0 = 1;
    PORTAbits.RA1 = 0;
    PORTB = digitos[unidades];
    __delay_ms(25);
    //decenas
    PORTAbits.RA0 = 0;
    PORTAbits.RA1 = 1;
    PORTB = digitos[decenas];
    __delay_ms(25);
    return;
}

void main(void) {
    TRISA = 0b00;
    TRISB = 0b0000000;
    uint8_t contador = 0;
    while (1) {
        for (int i = 0; i < 10; i++) {
            mostrar(contador);
        }
        if (contador != 99)
            contador++;
        else
            contador = 0;
    }
    return;
}
