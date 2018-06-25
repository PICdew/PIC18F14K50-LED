/*
 * File:   main.c
 *
 * Created on September 7, 2013, 1:56 PM
 */


#include <xc.h>
#include <stdint.h>

// PIC18F14K50
#pragma config FOSC   = HS,  PLLEN  = ON
#pragma config CPUDIV = NOCLKDIV
#pragma config HFOFST = OFF
#pragma config STVREN = ON,  LVP    = OFF

#define _XTAL_FREQ 12000000         // CLK 12MHz (use from __delay_ms)

volatile unsigned char out = 0;

void init(void)
{
    //OSCCON = 0b01010010;         // internal osc 4MHz
    
    TRISC  = 0b00000000;            // direction: all out
    ANSEL  = 0b00000000;            // analog/digital: all digital
    ANSELH = 0b00000000;            // analog/digital: all digital
}

void main(void)
{
    init();

    for (uint16_t i = 0; i < 24 * 30; ++i) {
        PORTC = 1;                 // High to port c
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");

        PORTC = 0;                 // Low to port c
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }

    return;
}
