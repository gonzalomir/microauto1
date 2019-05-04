#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

void main(void) {
    TRISA = 0b11;
    TRISB = 0x00;
    PORTB = 0;
    while (1) {
        if (PORTAbits.RA0) {
            PORTBbits.RB0 = 1;
        } else {
            PORTBbits.RB0 = 0;
        }
    }
    return;
}
