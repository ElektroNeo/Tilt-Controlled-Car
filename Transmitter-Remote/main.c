/*
 * File:   main.c
 * Author: bahti
 *
 * Created on 26 Mart 2019 Sal?, 22:08
 */

#include "config.h"

void main(void) {
    int Ax,Ay,Az,T,Gx,Gy,Gz;
	double Xa,Ya,Za,t,Xg,Yg,Zg;
    
    int16_t num = 100;
    char buffer[60];
    TRISB = 0;
    ANSELB = 0;
    PORTB = 0;
    
    I2C_Init();
	MPU6050_Init();
    Nokia_Init();
    nRF_Init(RX_MODE, 0x40);
    
    Nokia_Clear();
    
    char bufferRX[32];
    bufferRX[0] = 0;
    
    while(1) {
        SSP1CON1bits.SSPEN = 1;
        T = (((int)I2C_Read(0x68, TEMP_OUT_H)) << 8) | ((int)(I2C_Read(0x68, TEMP_OUT_L)));
        Gx = (((int)I2C_Read(0x68, GYRO_XOUT_H)) << 8) | ((int)(I2C_Read(0x68, GYRO_XOUT_L)));
        Gy = (((int)I2C_Read(0x68, GYRO_YOUT_H)) << 8) | ((int)(I2C_Read(0x68, GYRO_YOUT_L)));
        Gz = (((int)I2C_Read(0x68, GYRO_ZOUT_H)) << 8) | ((int)(I2C_Read(0x68, GYRO_ZOUT_L)));
        
        Ax = (((int)I2C_Read(0x68, ACCEL_XOUT_H)) << 8) | ((int)(I2C_Read(0x68, ACCEL_XOUT_L)));
        Ay = (((int)I2C_Read(0x68, ACCEL_YOUT_H)) << 8) | ((int)(I2C_Read(0x68, ACCEL_YOUT_L)));
        Az = (((int)I2C_Read(0x68, ACCEL_ZOUT_H)) << 8) | ((int)(I2C_Read(0x68, ACCEL_ZOUT_L)));
        
        Xg = (double)Gx/131.0;
        Yg = (double)Gy/131.0;
        Zg = (double)Gz/131.0;
        
        Xa = (double)Ax/16384.0;
        Ya = (double)Ay/16384.0;
        Za = (double)Az/16384.0;
        
        t = ((double)(T/340.00))+36.53;
        SSP1CON1bits.SSPEN = 0;
        __delay_ms(200);
        SSP2CON1bits.SSPEN = 1;
        if(nRF_DataReady()){
            nRF_ReadData(bufferRX);
            PORTB = bufferRX[0];
        }
        
        __delay_ms(50);
        
        sprintf(buffer, "Ax: %.2f \nAy: %.2f \nAz: %.2f \nT: %.2f \nData: %c $", (Xa*(-90)), (Ya*(-90)), (Za*(-90)), t, bufferRX[0]);
        //sprintf(buffer, "\n%c $", bufferRX[0]);
        Nokia_SendString(buffer);
        //Nokia_PositionXY(2, 2);   // Set position to next char
        //Nokia_SendData(bufferRX[0]);     // Send next char data
        SSP2CON1bits.SSPEN = 0;
    }
}
