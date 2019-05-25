#define _XTAL_FREQ 4000000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include "lcd.h"

__bit old2, new2;
__bit old3, new3;

void mostrarNum(uint8_t fil, uint8_t col, int num) {
    char aux[4];
    int aux2;
    LCD_Set_Cursor(fil, col - 4);
    LCD_putrs("     ");
    itoa(aux, num, 10);
    if (num > 9) {
        aux2 = numCifras(num) - 1;
        col = col - aux2;
    }
    if(num < 0){
        aux2 = numCifras(num);
        col = col - aux2;
    }
    LCD_Set_Cursor(fil, col);
    LCD_puts(aux);

    return;
}

int numCifras(int num) {
    int contador = 1;
    while (num / 10 > 0) {
        num = num / 10;
        contador++;
    }
    return contador;
}

void main(void) {
    LCD lcd = {&PORTB, 2, 3, 4, 5, 6, 7}; // PORT, RS, EN, D4, D5, D6, D7
    LCD_Init(lcd);
    TRISB = 0B00000011;
    OPTION_REGbits.nRBPU = false;
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Contador 0 a 100");
    int a = 0;

    for (;;) {
        new2 = PORTBbits.RB0; // Aumentar
        new3 = PORTBbits.RB1; // Disminuir
        if (old2 && !new2) {
            a++;
            mostrarNum(1, 10, a);
        }
        if (old3 && !new3) {
            a--;
            mostrarNum(1, 10, a);
        }
        old2 = new2;
        old3 = new3;
    }
    return;
}
