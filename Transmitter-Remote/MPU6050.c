/*
 * File:   MPU6050.c
 * Author: bahti
 *
 * Created on 26 Mart 2019 Sal?, 22:33
 */


#include "MPU6050.h"

void I2C_Init()
{
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    TRISCbits.RC3 = 1;      // SCK1
	TRISCbits.RC4 = 1;      // SDA1
	SSP1STATbits.SMP = 1;
    SSP1CON1bits.SSPEN = 1;
    SSP1CON1bits.SSPM = 0b1000;
    SSP1CON3bits.PCIE = 1;
    SSP1CON3bits.SCIE = 1;
    SSP1CON3bits.SDAHT = 1;
    SSP1ADD = BITRATE;               /* Set clock frequency */  
    /*
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSP1IE = 1;
    PIR1bits.SSP1IF = 0; */
}

void I2C_Write(unsigned char address, unsigned char reg, unsigned char data) {
    unsigned char junk;
    // Start condition
    SSP1CON2bits.SEN = 1;
    I2C_Ready();
    // Send slave address.
    address = (address << 1) | 0;   // Set to write.
    SSP1BUF = address;
    I2C_Ready();
    // Send register address.
    SSP1BUF = reg;
    I2C_Ready();
    // Send data.
    SSP1BUF = data;
    I2C_Ready();
    // Stop condition
    SSP1CON2bits.PEN = 1;
    I2C_Ready();
    junk = SSP1BUF;
}

unsigned char I2C_Read(unsigned char address, unsigned char reg) {
    unsigned char receivedData;
    
    // Start condition
    SSP1CON2bits.SEN = 1;
    I2C_Ready();
    
    // Send slave address.
    address = (address << 1) | 0;   // Set to write.
    SSP1BUF = address;
    I2C_Ready();
    
    // Send register address.
    SSP1BUF = reg;
    I2C_Ready();
    
    // Restart condition
    SSP1CON2bits.SEN = 1;
    I2C_Ready();
    
    // Send slave address.
    address = address | 1;   // Set to read.
    SSP1BUF = address;
    I2C_Ready();

    // Receive enable
    SSP1CON2bits.RCEN = 1;
    I2C_Ready();
    
    // Receive data
    receivedData = SSP1BUF;
    I2C_Ready();
    
    // Send ACK=1 to stop receiving.
    SSP1CON2bits.ACKDT = 1;
    SSP1CON2bits.ACKEN = 1;
    I2C_Ready();
    
    // Stop condition
    SSP1CON2bits.PEN = 1;
    I2C_Ready();
    
    return receivedData;
}

void I2C_Ready() {
    //while(!PIR1bits.SSP1IF);
    //PIR1bits.SSP1IF = 0;
    while ((SSP1CON2 & 0b00011111) || (SSP1STAT & 0b00000100));
}

void MPU6050_Init()										/* Gyro initialization function */
{ 
	__delay_ms(250);		// Power up time >100ms
	// 1KHz sample rate
    I2C_Write(0x68, SMPLRT_DIV, 0x07);
    // X axis gyroscope reference frequency
    I2C_Write(0x68, PWR_MGMT_1, 0x01);
    // Fs = 8KHz
    I2C_Write(0x68, CONFIG, 0x00);
    // Full scale range +/- 2000 degree/C
    I2C_Write(0x68, GYRO_CONFIG, 0x18);
    // Write to interrupt enable register
    I2C_Write(0x68, INT_ENABLE, 0x01);
}