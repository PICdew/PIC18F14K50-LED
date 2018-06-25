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

#define HIGH() \
  do { \
    PORTC = 1; \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    PORTC = 0; \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
  } while(0)

#define LOW() \
  do { \
    PORTC = 1; \
    asm("nop"); \
    asm("nop"); \
    PORTC = 0; \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
    asm("nop"); \
  } while(0)

const uint32_t led[30] = {
    0x100000, 0x200000, 0x300000, 0x400000, 0x500000, 0x600000, 0x700000, 0x800000, 0x900000, 0xa00000,
    0x001000, 0x002000, 0x003000, 0x004000, 0x005000, 0x006000, 0x007000, 0x008000, 0x009000, 0x00a000,
    0x000010, 0x000020, 0x000030, 0x000040, 0x000060, 0x000070, 0x000080, 0x000080, 0x000090, 0x0000a0
};

void main(void)
{
    init();

    for (uint8_t i = 0; i < sizeof(led) / sizeof(led[0]); ++i) {
        uint32_t mask = 0x800000;
        uint32_t l = led[i];
        
        while (mask != 0) {
            if ((l & mask) != 0) HIGH();
            else LOW();

            mask >>= 1;
        }
    }
    
    return;
}
