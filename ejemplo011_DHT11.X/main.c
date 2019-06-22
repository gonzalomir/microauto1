// DEFINES 
#define _XTAL_FREQ 4000000 // Telling the compiler, that we are using 4MHz 
#define data_dht PORTAbits.RA2 // Defining RA0 as datapin 
#define data_dht_dir TRISAbits.TRISA2 // Definig TRISA0 as dataport 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <xc.h>
#include "lcd.h"

/*
#pragma config FOSC = INTOSCIO // Oscillator Selection bits (INTRC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN) 
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled) 
#pragma config PWRTE = ON // Power-up Timer Enable bit (PWRT enabled) 
#pragma config MCLRE = ON // RA5/MCLR pin function select (RA5/MCLR pin function is MCLR) 
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOD Reset enabled) 
#pragma config LVP = ON // Low-Voltage Programming Enable bit (RB4/PGM pin has PGM function, low-voltage programming enabled) 
#pragma config CPD = OFF // Data Code Protection bit (Data memory code protection off) 
#pragma config CP = OFF // Code Protection bits (Program memory code protection off) 
 */

// Prototipos 
void StartSignal(void);
unsigned short ReadByte();
unsigned short CheckResponse();

//GLOBAL VARIABLES 
char message1[] = "Temp = 00.0 c";
char message2[] = "RH =   00.0 %";
unsigned short TOUT = 0, CheckSum, i;
unsigned short T_Byte1, T_Byte2, RH_Byte1, RH_Byte2;

void __interrupt(high_priority) tcInt(void) {
    if (PIR1bits.TMR2IF) { // If TMR2 to PR2 match Interrupt Flag     
        TOUT = 1;
        T2CONbits.TMR2ON = 0; // Stop timer
        PIR1bits.TMR2IF = 0; // Clear TMR0 interrupt flag
    }
}

void main(void) {
    unsigned short check;
    TRISB = 0b00000000; // TRISB output
    PORTB = 0b00000000; // PORTB low
    TRISA = 0b00000001; // TRISA output
    PORTA = 0b00000000; // PORTA low
    LCD lcd = {&PORTB, 2, 3, 4, 5, 6, 7}; // PORT, RS, EN, D4, D5, D6, D7
    CMCON = 0x07; // Comparators off

    // TIMER
    INTCONbits.GIE = 1; // Enable global interrupt
    INTCONbits.PEIE = 1; // Enable peripheral interrupt
    PIE1bits.TMR2IE = 1; // Enable Timer2 interrupt
    T2CON = 0; // Prescaler 1:1 and Timer2 is off initially
    PIR1bits.TMR2IF = 0; // Clear TMR INT flag bit
    TMR2 = 0;

    LCD_Init(lcd);
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Bienvenido");
    __delay_ms(250);
    LCD_Clear();

    for (;;) {
        __delay_ms(1000);
        StartSignal(); // Send the Startsignal
        check = CheckResponse(); // Assign check with 0 = OK, or 1 = NOT OK
        if (!check) { // OK check = 1 => NOT OK
            LCD_Set_Cursor(0, 0);
            LCD_putrs("No response."); // Write error message
            LCD_Set_Cursor(1, 0);
            LCD_putrs("Please check.");
        } else { // IF chack = 0 => OK
            RH_Byte1 = ReadByte(); // Read first byte
            RH_Byte2 = ReadByte(); // Read second byte
            T_Byte1 = ReadByte(); // Read third byte
            T_Byte2 = ReadByte(); // Read fourth byte
            CheckSum = ReadByte(); // Read checksum
            // Checks if all bytes is equal to the checksum
            if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)) {
                message1[7] = T_Byte1 / 10 + 48; // Extract the tens place
                message1[8] = T_Byte1 % 10 + 48; // Extract the ones place
                message1[10] = T_Byte2 / 10 + 48; // Extract the decimal
                message1[11] = 223; // ASCII code for degree symbol          
                message2[7] = RH_Byte1 / 10 + 48; // Extract the tens place
                message2[8] = RH_Byte1 % 10 + 48; // Extract the ones place
                message2[10] = RH_Byte2 / 10 + 48; // Extract the decimal
                LCD_Clear();
                LCD_Set_Cursor(0, 0);
                LCD_puts(message1); // Write the temp to LCD
                LCD_Set_Cursor(1, 0);
                LCD_puts(message2); // Write the humidity to LCD
            } else { // Checksum is not correct
                LCD_Set_Cursor(0, 0);
                LCD_putrs("Checksum error!");
                LCD_Set_Cursor(1, 0);
                LCD_putrs("Please wait.");
            }
        }
    }
    return;
}

void StartSignal() {
    data_dht_dir = 0; // Sets TRISA2 to output 
    data_dht = 0; // Set RA2 to LOW 
    __delay_ms(18); // Waits for 18 ms 
    data_dht = 1; // Sets RA2 HIGH 
    __delay_us(20); // Waits for 20 ms 
    data_dht_dir = 1; // Sets TRISA2 to input 
}

unsigned short ReadByte() {
    unsigned short num = 0, t;
    data_dht_dir = 1; // Sets TRISA2 to input
    for (i = 0; i < 8; i++) { // Start loop
        while (!data_dht); // When data is not valid
        TMR2 = 0; // Sets TMR2 to 0
        T2CONbits.TMR2ON = 1; // Start TMR2 from 0 when a low to high data pulse
        while (data_dht); // is detected, and wait until it falls low again
        T2CONbits.TMR2ON = 0; // Stop the TMR2 when the data pulse falls low
        if (TMR2 > 40) num |= 1 << (7 - i); // If time > 40us, data is 1
    }
    return num; // Return 8-bit = 1-byte
}

unsigned short CheckResponse() {
    TOUT = 0;
    TMR2 = 0;
    T2CONbits.TMR2ON = 1; // Turn on TMR2
    while (!data_dht & !TOUT); // While NOT data and NOT TOUT
    if (TOUT)
        return 0; // Return 0 => OK
    else {
        TMR2 = 0; // Disable Timer 2 
        while (data_dht & !TOUT); // While data and NOT TOUT 
        if (TOUT)
            return 0; // If Tout = 1 then return 0 => OK 
        else {
            T2CONbits.TMR2ON = 0; // Turn off TMR2 
            return 1; // Return 1 => NOT OK 
        }
    }
}