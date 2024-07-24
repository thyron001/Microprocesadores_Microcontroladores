#include <xc.h>
#include "comi2c.h"



// Funciones para la comunicacion I2C    

void I2C_Init(void){
   
    
    SSPSTATbits.SMP = 1; // 100Kbps
    SSPSTATbits.CKE = 0; // Habilita las entradas específicas del SMBus.
    
    
    SSPCON1bits.SSPEN = 1; // Configura la comunicacion SDA Y SCL
    SSPCON1bits.SSPM = 0b1000;// Se establece en modo Maestro
    
    SSPCON2 = 0x00;  // Revisar estos registros 
    
    SSPADD = 29;// Fosc/4/BitRate - 1  

};

void I2C_Start(void){
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN == 1);
};

void I2C_Stop(void){
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN == 1);
};

void I2C_Restart(void){
    SSPCON2bits.RSEN = 1;
    while(SSPCON2bits.RSEN == 1);
};

void I2C_Ack(void){
    PIR1bits.SSPIF = 0;
    SSPCON2bits.ACKDT = 0;//ACK
    SSPCON2bits.ACKEN = 1;//HABILITAR
    while(PIR1bits.SSPIF == 0);
};

void I2C_Nack(void){
    PIR1bits.SSPIF = 0; // interrupcion 
    SSPCON2bits.ACKDT = 1;//NACK
    SSPCON2bits.ACKEN = 1;//HABILITAR
    while(PIR1bits.SSPIF == 0);
};

void I2C_Tx(char data){
    PIR1bits.SSPIF = 0;
    SSPBUF = data;
    while(PIR1bits.SSPIF == 0);
};

char I2C_Rx(void){
    PIR1bits.SSPIF = 0;
    SSPCON2bits.RCEN = 1;  // habilita la recepcion 
    while(PIR1bits.SSPIF == 0);
    return SSPBUF;
};