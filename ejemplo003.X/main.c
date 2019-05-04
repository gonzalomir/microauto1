// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = ON         // Data EE Memory Code Protection bit (Data memory code-protected)
#pragma config CP = ON          // Flash Program Memory Code Protection bit (0000h to 07FFh code-protected)

#define _XTAL_FREQ 4000000
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>

void main(void) {
    TRISB = 0x00;
    uint8_t x = 0B00001111; //1 en decimal
    bool direccion = 0; //0: Izquierda y 1: Derecha
    while (1) {
        PORTB = x;
        if (direccion) {
            x = x >> 1;
        } else {
            x = x << 1;
        }
        __delay_ms(200);
        if (x == 0) {
            direccion = !direccion;
            x = direccion ? 0B11110000 : 0B00001111; //192:3 en decimal
        }
    }
    return;
}