#define _XTAL_FREQ 4000000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "lcd.h"

void main(void) {
    LCD lcd = {&PORTB, 2, 3, 4, 5, 6, 7}; // PORT, RS, EN, D4, D5, D6, D7
    LCD_Init(lcd);
    TRISB = 0B00000011;
    OPTION_REGbits.nRBPU = false;
    LCD_Clear();
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Int a String");
    int a = 1;
    float b = 0.5;
    char aux[3];

    char rxbuffer[20];
    int status;
    char *text;
    float value;
    value = 123.456;

    //strcpy(rxbuffer,text);

    for (;;) {
        LCD_Set_Cursor(1, 0);
        itoa(aux, a, 10);
        LCD_puts(aux);
        a++;

        LCD_Set_Cursor(1, 5);
        text = ftoa(value, &status);
        LCD_puts(text);

        __delay_ms(250);

    }
    return;
}
