/*
 * File:   PIC16F676MOTOROPEN.c
 * Author: aswin
 *
 * Created on 23 May, 2024, 12:07 PM
 */


#include <xc.h>


#pragma config FOSC = XT  // Oscillator Selection bits (Internal oscillator)
#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF     // RA3/MCLR pin function is digital input, MCLR internally tied to VDD
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR enabled)
#pragma config CP = OFF        // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF       // Data Code Protection bit (Data memory code protection is disabled)

#define _XTAL_FREQ 4000000  // Define the operating frequency

void init_ADC() {
    ANSEL = 0b00000001;  // Set AN0 as analog input
    ADCON0 = 0b00000001; // ADCON0: ADON=1, Channel=AN0
    ADCON1 = 0b10000000; // ADCON1: ADFM=1 (right justified), ADCS=Fosc/2
}

unsigned int read_ADC() {
    __delay_us(10);         // Acquisition time
    ADCON0bits.GO_nDONE = 1; // Start conversion
    while (ADCON0bits.GO_nDONE); // Wait for conversion to complete
    return ((ADRESH << 8) + ADRESL); // Return 10-bit result
}

void main() {
    // Initialize
    TRISA = 0b00000001; // Set RA0 as output, AN0 as input
    TRISC = 0;          // Set PORTC as output
    PORTA = 0;          // Clear PORTA
    PORTC = 0;          // Clear PORTC
    init_ADC();         // Initialize ADC

    while (1) {
        unsigned int sensorValue = read_ADC(); // Read sensor value

        if (sensorValue > 512) {
            PORTAbits.RA0 = 1; // Open valve (set RA0 high)
        } else {
            PORTAbits.RA0 = 0; // Close valve (set RA0 low)
        }

        __delay_ms(100); // Delay for debounce/stability
    }
}
