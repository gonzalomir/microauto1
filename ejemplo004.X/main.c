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

void main(void) {
    TRISB = 0x00;
    while (1) {
        for (uint8_t i = 0; i < 16; i++) {
            PORTB = digitos[i];
            __delay_ms(400);
        }
    }
    return;
}
