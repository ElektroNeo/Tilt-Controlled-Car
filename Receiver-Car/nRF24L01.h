/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: Bahtiyar Bayram
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NRF24L01
#define	NRF24L01

#include <xc.h> // include processor files - each processor file is guarded.  

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#define CE_RF           PORTDbits.RD3
#define CSN_RF          PORTDbits.RD2
#define MOSI_RF         PORTCbits.RC4   // SDI Pin
#define MISO_RF         PORTCbits.RC5   // SDO Pin
#define SCK_RF          PORTCbits.RC3   // SCK Pin

#define CE_RF_DDR       TRISDbits.TRISD3
#define CSN_RF_DDR      TRISDbits.TRISD2
#define MOSI_RF_DDR     TRISCbits.TRISC4
#define MISO_RF_DDR     TRISCbits.TRISC5
#define SCK_RF_DDR      TRISCbits.TRISC3

void nRF_Init(char, char);
void nRF_WriteRegister(char, char);
char nRF_ReadRegister(char);
void SPI_Write(char);
char SPI_Read(void);
void nRF_WriteBuffer(char, char *, char);
void nRF_ReadBuffer(char, char *, char);
void nRF_SetMode(char);
void nRF_Flush(void);
void nRF_SendData(char *);
void nRF_ReadData(char *);
char nRF_DataReady(void);
void nRF_SetChannel(char);
char nRF_GetChannel(void);
void nRF_StandByI(void);


#define PAYLOAD_BYTES       5 // Sets the bytes to send or read.
#define RX_MODE             1
#define TX_MODE             0

//==============================================================================
// Register Map.
//==============================================================================
#define R_REGISTER          0x00  
#define W_REGISTER          0x20  
#define R_RX_PAYLOAD        0x61  
#define W_TX_PAYLOAD        0xA0  
#define FLUSH_TX            0xE1  
#define FLUSH_RX            0xE2  
#define REUSE_TX_PL         0xE3  

#define CONFIG             0x00  
#define EN_AA               0x01  
#define EN_RXADDR           0x02  
#define SETUP_AW            0x03  
#define SETUP_RETR          0x04  
#define RF_CH               0x05  
#define RF_SETUP            0x06  
#define STATUS_N            0x07  
#define OBSERVE_TX          0x08  
#define CD                  0x09  
#define RX_ADDR_P0          0x0A  
#define RX_ADDR_P1          0x0B  
#define RX_ADDR_P2          0x0C  
#define RX_ADDR_P3          0x0D  
#define RX_ADDR_P4          0x0E  
#define RX_ADDR_P5          0x0F  
#define TX_ADDR             0x10  
#define RX_PW_P0            0x11  
#define RX_PW_P1            0x12  
#define RX_PW_P2            0x13  
#define RX_PW_P3            0x14  
#define RX_PW_P4            0x15  
#define RX_PW_P5            0x16  
#define FIFO_STATUS         0x17  

#endif	/* XC_HEADER_TEMPLATE_H */

