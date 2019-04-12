/*
 * File:   nRF24L01.c
 * Author: BAhtiyar Bayram
 *
 * Created on 29 Mart 2019 Cuma, 22:41
 */


#include "nRF24L01.h"


// This data type sets the address data pipe 0.
unsigned char ADDRESS_DATA_PIPE0[5] = {0x05,0x04,0x03,0x02,0x01};

void nRF_Init(char mode, char rf_channel) {
    // SPI
    SCK_RF_DDR = 0;
    MISO_RF_DDR = 0;
    MOSI_RF_DDR = 1;
    SSPSTAT = 0xC0;
    SSPCON = 0x20;
    
    //nRF
    CSN_RF_DDR = 0;
    CE_RF_DDR = 0;
    CSN_RF = 1;
    CE_RF = 0;
    __delay_ms(100);
    
    nRF_WriteRegister(CONFIG, 0x0E); // Power Up, CRC Enable, CRC 2 byte
    __delay_ms(10);
    nRF_WriteRegister(EN_AA, 0x01); // Enable RX Address (data pipe 0)
    nRF_WriteRegister(SETUP_AW, 0x03); // RX/TX Address field width is 5 bytes
    nRF_WriteRegister(SETUP_RETR, 0x00); // Auto Retransmit Delay = 250us
    // Sets the frequency channel nRF24L01+ operates on
    nRF_WriteRegister(RF_CH, rf_channel);
    nRF_WriteRegister(RF_SETUP, 0x06); // RF Data Rate = 1Mbs, RF output power in TX mode = 0dBm

    nRF_WriteBuffer(W_REGISTER | RX_ADDR_P0, ADDRESS_DATA_PIPE0, 5);
    nRF_WriteBuffer(W_REGISTER | TX_ADDR, ADDRESS_DATA_PIPE0, 5);
    nRF_WriteRegister(RX_PW_P0, PAYLOAD_BYTES);
    __delay_ms(100);
    nRF_SetMode(mode);
    __delay_ms(100);
}

void nRF_WriteRegister(char mnemonic, char value) {
    CSN_RF = 0;        // Enable chip
    
    SPI_Write(W_REGISTER | mnemonic);
    SPI_Write(value);
    
    CSN_RF = 1;        // Disable chip
}

char nRF_ReadRegister(char mnemonic) {
    char data;
    CSN_RF = 0;        // Enable chip
    SPI_Write(R_REGISTER | mnemonic);
    data = SPI_Read();
    CSN_RF = 1;        // Disable chip
    return data;
}

void SPI_Write(char data) {
    SSPBUF = data;           // Copy data in SSBUF to transmit
    while(!SSPSTATbits.BF);     // Wait for complete 1 byte transmission
    data = SSPBUF;         // Clear SSP2IF flag
}

char SPI_Read() {
    SSPBUF = 0x00;           // Copy data in SSBUF to transmit
    while(!SSPSTATbits.BF);     // Wait for complete 1 byte transmission
    return SSPBUF;
}

void nRF_WriteBuffer(char data, char* buffer, char bytes) {
    CSN_RF = 0;         // Enable chip
    SPI_Write(data); 
    for(char i = 0; i < bytes; i++) {
        SPI_Write(*buffer);
        buffer++;
    }
    CSN_RF = 1;         // Disable chip
}

void nRF_ReadBuffer(char data, char* buffer, char bytes) {
    CSN_RF = 0;         // Enable chip
    SPI_Write(data); 
    for(char i = 0; i < bytes; i++) {
        *buffer = SPI_Read();
        buffer++;
    }
    *buffer = 0; // NULL char
    CSN_RF = 1;         // Disable chip
}

void nRF_SetMode(char mode) {
    nRF_Flush();        // Clear RX and TX FIFO
    nRF_WriteRegister(STATUS_N, 0x70); // Clear STATUS
    if(mode) {
        nRF_WriteRegister(CONFIG, 0x0F);  // RX Control
        CE_RF = 1;  // RX Mode
    } else {
        nRF_WriteRegister(CONFIG, 0x0E);  // TX Control
        CE_RF = 0;  // TX Mode
    }
}

void nRF_Flush() {
    CSN_RF = 0;         // Enable chip
    SPI_Write(FLUSH_TX);
    CSN_RF = 1;         // Disable chip
    CSN_RF = 0;         // Enable chip
    SPI_Write(FLUSH_RX);
    CSN_RF = 1;         // Disable chip
}

void nRF_SendData(char* buffer) {
    nRF_SetMode(TX_MODE);
    nRF_WriteBuffer(W_TX_PAYLOAD, buffer, PAYLOAD_BYTES);
    CE_RF = 1;      // RX Mode
    __delay_ms(1);
    CE_RF = 0;      // TX Mode
}

void nRF_ReadData(char* buffer) {
    nRF_ReadBuffer(R_RX_PAYLOAD, buffer, PAYLOAD_BYTES);
    nRF_WriteRegister(STATUS_N, 0x70); // Clear STATUS
    nRF_Flush();
}

char nRF_DataReady() {
    if((nRF_ReadRegister(STATUS_N) & 0x40) == 0x40) {
        return 1; 
    }
    return 0;
}

void nRF_SetChannel(char rf_channel) {
    nRF_WriteRegister(RF_CH, rf_channel);
}

char nRF_GetChannel() {
    return nRF_ReadRegister(RF_CH);
}

void nRF_StandByI() {
    nRF_WriteRegister(CONFIG, 0x0A);
    __delay_ms(10);
}