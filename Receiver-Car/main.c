/*
 * File:   main.c
 * Author: Bahtiyar Bayram
 *
 * Created on 29 Mart 2019 Cuma, 15:17
 */


#include "config.h"

void main(void) {
    
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 1;
    
    TRISB = 0;
    PORTB = 0;
    ANSEL = 0;
    ANSELH = 0;
    
    nRF_Init(TX_MODE, 0x40);
    char bufferTX[32];
    
    char bufferRX[32];
    
    while(1) {
        for(char i = 65; i < 91; i++) {
            bufferTX[0] = i;
            nRF_SendData(bufferTX);
            __delay_ms(100);
        }
    }
}
