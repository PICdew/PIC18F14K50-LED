#include <xc.h>

#pragma config FOSC = HS
#pragma config PLLEN  = ON
#pragma config CPUDIV = NOCLKDIV
#pragma config HFOFST = OFF
#pragma config STVREN = ON
#pragma config LVP = OFF

#define _XTAL_FREQ 48000000         // CLK 48MHz

void init(void) {
    TRISCbits.RC0 = 0;            // RC0 to output
    ANSELbits.ANS4 = 0;           // RC0 to digital
}

void main(void) {
    init();

    while (1) {
        PORTCbits.RC0 = !PORTCbits.RC0; // Toggle RC0

        __delay_ms(500);
    }
}
