/*
 * File:   Nokia5110.c
 * Author: Bahtiyar Bayram
 *
 * Created on 03 Mart 2019 Pazar, 16:03
 */

#include "NokiaLCD.h"

void Nokia_Init(void) {
    ANSELD = 0;         //  Disable ADC on PORTD
    DDR_DC = 0;         // Data - Command pin - DC=1 -Data, DC=0 -Command
    DDR_RST = 0;        // Reset pin (RST))
    DDR_SCE = 0;
    __delay_ms(100);
    RST = 0;            // Enable reset
    __delay_ms(100);
    RST = 1;            // Disable reset
    
    // PORT definition for SPI pins
    DDR_SCK2 = 0;       // RD0 as output(SCK2)
    DDR_SDO2 = 0;       // RD4 as output(SDO2)
    
    SEN = 1;            // Disable chip
    
    /*
    SSP2CON1bits.SSPM = 0b0000;     // SPI Master mode, clock = FOSC/64
    SSP2CON1bits.CKP = 0;           // Idle state for clock is a low level
    SSP2CON1bits.SSPEN = 1;         // Enables serial port and configures SCK, SDO, SDI and SS as the source of the serial port pins
    SSP2STATbits.CKE = 1;           // Data transmitted on rising edge of SCK
    SSP2STATbits.SMP = 1;*/
    
    SSP2STAT = 0xC0;
    SSP2CON1 = 0x20;
    
    // Enable SPI interrupt
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE3bits.SSP2IE = 1;
    PIR3bits.SSP2IF = 0; 
    
    // display extended commands
    Nokia_SendCommand(0x21);
    
    // select Bias voltage
    Nokia_SendCommand(0x13);
    
    // set temperature coefficient
    Nokia_SendCommand(0x07);
    
    // set LCD Vop for contrast
    Nokia_SendCommand(0xC0);
    
    // display basic commands
    Nokia_SendCommand(0x20);
    
    // set normal mode
    Nokia_SendCommand(0x0C);
}

void Nokia_SendCommand(char command) {
    DC = 0;         // Send command
    SCE = 0;        // Enable chip
    
    SSP2BUF = command;           // Copy data in SSBUF to transmit
    while(!PIR3bits.SSP2IF);     // Wait for complete 1 byte transmission
    PIR3bits.SSP2IF = 0;         // Clear SSP2IF flag
    
    SCE = 1;        // Disable chip
}

void Nokia_SendData(char data) {
    DC = 1;        // Send data
    SCE = 0;       // Enable chip
                
    for(char i = 0; i < 5; i++) {
        SSP2BUF = font[data-(0x20)][i];     // Copy data in SS2BUF to transmit
        while(!PIR3bits.SSP2IF);            // Wait for complete 1 byte transmission
        PIR3bits.SSP2IF = 0;                // Clear SSP2IF flag
    }
    
    SCE = 1;       // Disable chip
}

void Nokia_Clear() {  
    SCE = 0;       // Enable chip
    DC = 1;        // Send Data
    for(char i=0; i<6; i++)
    {   
        for(char j=0; j<84; j++) {
            SSP2BUF = 0;                 // Copy data in SS2BUF to transmit
            while(!PIR3bits.SSP2IF);     // Wait for complete 1 byte transmission
            PIR3bits.SSP2IF = 0;         // Clear SSP2IF flag
        }  
    }      
    SCE = 1;       // Disable chip
}

void Nokia_PositionXY(char x, char y) {
    Nokia_SendCommand(0x80 | (x*5));   // Set X position
    Nokia_SendCommand(0x40 | y);       // Set Y position
}

void Nokia_SendString(char *str) {
    Nokia_Clear();                // Clear LCD
    for(char x = 0, y = 0; *str != '$';) {
        
        if((*str) == '\n') {
            y++;
            x = 0;
            str++;
        }
        // If columns are full then write to next row
        else if(x == 15) {
            y++;                  // Add 1 to y position
            x = 0;                // Reset the x position
        }
        Nokia_PositionXY(x, y);   // Set position to next char
        Nokia_SendData(*str);     // Send next char data
        str++;                   // Set next char
        x++;                      // Add 1 to x position
    }
}