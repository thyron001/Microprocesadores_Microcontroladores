/*
 * File:   main_reloj.c
 * Author: javier Sotamba
 *
 * Created on 23 de julio de 2024, 2:49
 */


// CONFIG1L
#pragma config PLLDIV = 3       // PLL Prescaler Selection bits (Divide by 3 (12 MHz oscillator input))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (HS oscillator, PLL enabled (HSPLL))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOR = ON         // Brown-out eset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "comi2c.h" // Libreria de la configuraciones de comunicacion I2C 
#include "lcd_confi.h" // Libreria para el LCD 

#define _XTAL_FREQ 48000000 // frecuencia de oscilacion 


char t_i[] = "121020";
char date[] = "100000";
uint8_t hours, minutes, seconds; 
uint8_t day, month, year; 

//*******************************************************************************

// Funciones para leer datos del RTC en base al Datasheet del 3231 

void read_time(void){
    
    I2C_Start();            //Incia comunicaión I2C
    I2C_Tx(0xD0);        //Escoje dirección del reloj
    I2C_Tx(0x00);        //Posición donde va leer
    I2C_Restart();          //Reinicia la comuniación I2C

    I2C_Tx(0xD1); // Read mode 
    I2C_Ack(); // ACKNOWLEDGE
    seconds = I2C_Rx();
    I2C_Ack(); // ACKNOWLEDGE
    minutes = I2C_Rx();
    I2C_Ack(); // ACKNOWLEDGE
    hours = I2C_Rx();
    I2C_Nack(); // NOT ACKNOWLEDGE
    I2C_Stop(); // STOP 
    
};



void write_time(void){
    hours = ((t_i[0] & 0x0F) << 4) + (t_i[1] & 0x0F);
    minutes = ((t_i[2] & 0x0F) << 4) + (t_i[3] & 0x0F);
    seconds = ((t_i[4] & 0x0F) << 4) + (t_i[5] & 0x0F);
    
    I2C_Start(); // START 
    I2C_Tx(0xD0); // Write mode
    I2C_Tx(0x00); // Posicion de los segundos 
    I2C_Tx(seconds); 
    I2C_Tx(minutes); 
    I2C_Tx(hours); 
    I2C_Ack();
    I2C_Stop();
  
};



void read_date(void){
   
    I2C_Start(); // Iniciar comunicación I2C
    I2C_Tx(0xD0); // Dirección en modo de escritura (para acceder a los registros de fecha)
    I2C_Tx(0x04);
    I2C_Restart();
    
    I2C_Tx(0xD1);
    
    
    day = I2C_Rx();
    I2C_Ack(); // ACKNOWLEDGE

    // Leer el mes
    month = I2C_Rx();
    I2C_Ack(); // ACKNOWLEDGE

    // Leer el año
    year = I2C_Rx();
    I2C_Nack(); // NOT ACKNOWLEDGE
    I2C_Stop(); // Detener comunicación I2C

};

void write_date(void){
    day = ((date[0] & 0x0F) << 4) + (date[1] & 0x0F);
    month = ((date[2] & 0x0F) << 4) + (date[3] & 0x0F);
    year = ((date[4] & 0x0F) << 4) + (date[5] & 0x0F);
    
    I2C_Start(); // START 
    I2C_Tx(0xD0); // Write mode
    I2C_Ack();
    I2C_Tx(0x04); // Posicion del dia
    I2C_Tx(day); 
    I2C_Ack();
    I2C_Tx(month); 
    I2C_Ack();
    I2C_Tx(year); 
    I2C_Ack();
    I2C_Stop();
  
};



//*******************************************************************************
void main(void) {
    
    
    // Configuraciones iniciales 
    ///////////////////////////////////////////////////////////////////////////
    // Configuracion de puerto C como entrada digital
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1; 
    
    // Configuracion de los puertos para la comunicacion I2C con el modulo RTC 
    TRISBbits.RB0 = 1; // SDA
    TRISBbits.RB1 = 1; // SCL
    TRISBbits.RB2 = 1; // Pulsantes 
    TRISBbits.RB3 = 1;
    TRISBbits.RB4 = 1; 
   
    //Puertos para el LCD
    TRISD = 0x00; // segmentos de LCD y LED de aviso para la alarmas y Buzzer   
    
    //Condiciones iniciales 
    LATD = 0x00; 
    LATC = 0x00; 
    
    
  
    //***************************End Setup**************************************
    
    I2C_Init(); // INICIO de la comunicacion I2C
    LCD_init(); // INICIO del LCD
    
    write_time(); 
    
    LATDbits.LD1=0;
    txbits(0x0C, 1, 1); 
    LATDbits.LD1=1;
    
    limpiar(); 
    __delay_ms(100);  
    
    mostrar_mensaje(" Hola  perrasos ");
    __delay_ms(1000);
    limpiar();
    LATDbits.LD1=1;
    
    while(1){
        
        read_time();
        mostrar_mensaje((hours>>4)+0x30);
        mostrar_mensaje((hours & 0x0F)+0x30);
        __delay_ms(100);
    
    
      
    };

};

