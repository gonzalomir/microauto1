#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

__bit estado = 0;

void main(void) {
    TRISA = 0b00001;
    TRISB = 0;
    PORTB = 0;

    while (1) {
        if (PORTAbits.RA0) {
            estado = !estado;
            while(PORTAbits.RA0);
        }
        PORTBbits.RB0 = estado;
    }
    return;
}