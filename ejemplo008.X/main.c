#define _XTAL_FREQ 4000000

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xc.h>

#include "lcd.h"

void main(void) {
    LCD lcd = {&PORTB, 2, 3, 4, 5, 6, 7}; // PORT, RS, EN, D4, D5, D6, D7
    LCD_Init(lcd);
    for (;;) {
        LCD_Clear();
        LCD_Set_Cursor(0, 0);
        LCD_putrs("ICEI Micro  ");

        LCD_Set_Cursor(1, 0);
        for (char c = 'A'; c < 'Q'; ++c) { // Print characters A-P
            LCD_putc(c);
            __delay_ms(100);
        }
        __delay_ms(100);
    }
    return;
}
