#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdint.h>

void main(void) {
    uint8_t contador = 0;
    TRISB = 0x00;
    PORTB = 0;
    while (1) {
        PORTB = contador;
        contador++;
        __delay_ms(200);
    }
    return;
}
