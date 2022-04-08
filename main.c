/*
 * lab07_lab 
 * File:   main.c
 * Author: Gerardo Paz - 20173
 * Contador en puerto A con botones en puerto B
 * 
 * Created on April 5, 2022, 8:52 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>         // registros del PIC
#include <stdio.h>
#include <stdlib.h>

#define _tmr0_value 158   // TMR0 value
#define _XTAL_FREQ 1000000  // oscilador 

uint8_t contador = 0;

void __interrupt() isr (void){
    if (RBIF){
        if(PORTBbits.RB0){      // ver qué pin ocasionó la interrupción
            contador++;
        }
        else if(PORTBbits.RB1){      // ver qué pin ocasionó la interrupción
            contador--;
        }
        RBIF = 0; // limpiar bandera
    }
    if(T0IF){
        TMR0 = _tmr0_value;
        PORTC++;
        T0IF = 0;       
    }
    return;
}

void timer0(void){
    // Timer0
    //OPTION_REG = 0b00000111;     // Timer0 con prescaler de 256
    OPTION_REGbits.T0CS = 0;    // temporizador
    OPTION_REGbits.PSA = 0;     // asignar prescaler
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;     // ps 1:256
    INTCONbits.T0IF = 0;
    TMR0 = _tmr0_value;
    return;
}

void setup_int(void){
    INTCON = 0b10101000; // globales, onchange y bandera limpia de RB y Timer0
    IOCB =  0b00000011;  // onchange de RB0 y RB1
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;  // PORTA out
    TRISC = 0;
    TRISB = 0b00000011; // RB0 in
    
    PORTA = 0;
    PORTC = 0;
    PORTB = 0;  // puertos limpios
    
    // oscilador
    OSCCONbits.IRCF = 0b0100;   // 4MHz
    OSCCONbits.SCS = 1;         // oscilador interno
    return;
}

void main(void){
    setup();
    setup_int();
    timer0();
    while(1){   // principal loop
        PORTA = contador;
    }
}

