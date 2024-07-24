/*
 * File:   calculadora.c
 * Author: javier Sotamba
 *
 * Created on 19 de junio de 2024, 22:41
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

#define _XTAL_FREQ 48000000 // frecuencia de oscilacion 

//variables 
const char posiciones[15] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E};

float n = 0.0; 
float dato1 = 0.0; 
float dato2 = 0.0; 
float aux = 0.0; 
float num = 0.0;
float resultado = 0.0;
float auxd = 0.0; 

int puntodecimal = 0; 
int shift = 0; 
int varcontrol = 0; 
int con = -1; 
int d = 0;
int posicion = 0;
int opcontrol = 0; 
int solouno = 0; 

char cadena[10] = "";
char ope; 

const char clave[4] = {0x01, 0x02, 0x03, 0x04};
int clave_0;
int clave_1;
int clave_2;
int clave_0_temp;
int clave_1_temp;
int clave_2_temp;

int clave_0_real = 1;
int clave_1_real = 2;
int clave_2_real = 3;

int bandera_clave = 0;
int clave_correcta = 0;



void delay_ms(char milliseconds) {
    while(milliseconds-- > 0) {
        __delay_ms(1); // Suponiendo que __delay_ms(1) es permitido
    }
}

void enable_pulse(){
    //enable = 1;
    LATDbits.LD2 = 1;
    __delay_us(1);
    //__delay_ms(1);
    //enable = 0;
    LATDbits.LD2= 0;
    __delay_us(1); 
}

void txbits(char dato, int d1, int d2){ 
    char temp;  
    //Se lee el LATD porque D es un puerto de salida, si fuera entrada se lee el PORTD
    //Enviar W
    temp = LATD & 0x0F; //Rotar 4 veces
    LATD = (dato & 0xF0) | temp;
    enable_pulse();
    delay_ms(d1);

    
    LATD = ((dato << 4) & 0xF0) | temp ; // Mascara para tomar los bits menos significantes
    enable_pulse();
    delay_ms(d2);
    
    //Enviar Z
  
}
void limpiar(void){
    LATDbits.LD1 = 0;
    txbits(0x01, 1, 1);
    __delay_ms(2);
    LATDbits.LD1 = 1;
}

void LCD_init(){
    __delay_ms(15);
    //RS = 0. Para enviar instruccion
    LATDbits.LD1 = 0;
    txbits(0x33, 5, 1);
    txbits(0x32, 1, 1);
    txbits(0x28, 1, 1);
    txbits(0x06, 1, 1);
    txbits(0x0E, 1, 1);
    txbits(0x01, 1, 1);
    __delay_ms(2); 
    txbits(0x80, 1, 1);
}
void mostrar_mensaje(const char *mensaje) {
    for (int i = 0; i < strlen(mensaje); i++) {
       if (i == 16){
        LATDbits.LD1 = 0;
        txbits(0xC0, 1, 1);
        LATDbits.LD1 = 1;
        }
        txbits(mensaje[i], 1, 1);
    }    
}


void recorrer_valores(void){
    clave_1_temp = clave_1;
    
	clave_1 = clave_2;
	clave_0 = clave_1_temp;
    return;
                
    }


void init(void){ //set up inicial 
     // perifericos configuracions
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1; 
    //**
    // E/S configuracion
    TRISD = 0x00; // segmentos de displays 
    TRISB = 0xF0;
    TRISE = 0x00;
   //**
    // Condiciones iniciales
    LATD = 0x00;
    LATE = 0x00;
    //**
}

void LCD_Data(unsigned char data) {
    LATDbits.LD1 = 1;
    txbits(data, 1, 1);
    LATDbits.LD1 = 1;
}

void main(void) {

    init();
     
    LCD_init();
    
    
    LATDbits.LD1=0;
    txbits(0x0C, 1, 1); 
    LATDbits.LD1=1;
    
    
     limpiar(); 
    __delay_ms(1000);
    
    mostrar_mensaje("Tyrone Novillo");
    
    LATDbits.LD1 = 0;
    txbits(0xC0, 1, 1);
    LATDbits.LD1 = 1;
    
    mostrar_mensaje("0106505464");
    
    for (int i = 0; i < 5; i++) {
        __delay_ms(1000);
    }    
    
    limpiar();
    
    for (int i = 0; i < 3; i++) {
        LATEbits.LATE0 = 1;
        __delay_ms(200);
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 1;
        __delay_ms(200);
        LATEbits.LATE1 = 0;
        LATEbits.LATE2 = 1;
        __delay_ms(200);
        LATEbits.LATE2 = 0;
        __delay_ms(200);
    }    
    
    while(1){ 
        
        if(PORTCbits.RC5 == 0){
            mostrar_mensaje("SISTEMA LISTO");
            
            for (int i = 0; i < 3; i++) {
                __delay_ms(1000);
            }
            limpiar();
            
            while (PORTCbits.RC5 == 1){
                mostrar_mensaje("Lectura ADC");            
            }
            
            limpiar();
            
            mostrar_mensaje("SISTEMA BLOQUEADO");
            LATDbits.LD1 = 0;
            txbits(0xC0, 1, 1);
            LATDbits.LD1 = 1;
            mostrar_mensaje("Clave:");
            while (bandera_clave == 0){
                LATB = 0x0E;
                switch (PORTB) {
                    case 0xEE:
                        recorrer_valores();
                        clave_2 = 1;
                        mostrar_mensaje("1");
                        break;
                    case 0xDE :
                        recorrer_valores();
                        clave_2 = 2;
                        mostrar_mensaje("2");
                        break;
                    case 0x7E :
                        recorrer_valores();
                        clave_2 = 3;
                        mostrar_mensaje("3");
                        break;
                  };
                 __delay_ms(50);

                LATB = 0x0D;
                switch (PORTB) {
                    case 0xED:
                        recorrer_valores();
                        clave_2 = 4;
                        mostrar_mensaje("4");
                        break;
                    case 0xDD :
                        recorrer_valores();
                        clave_2 = 5;
                        mostrar_mensaje("5");
                        break;
                    case 0x7D :
                        recorrer_valores();
                        clave_2 = 6;
                        mostrar_mensaje("6");
                        break;
                  };
                 __delay_ms(50);

                 LATB = 0x0B;
                switch (PORTB) {
                    case 0xEB:
                        recorrer_valores();
                        clave_2 = 7;
                        mostrar_mensaje("7");
                        break;
                    case 0xDB :
                        recorrer_valores();
                        clave_2 = 8;
                        mostrar_mensaje("8");
                        break;
                    case 0x7B :
                        recorrer_valores();
                        clave_2 = 9;
                        mostrar_mensaje("9");
                        break;
                  };
                 __delay_ms(50);


                  LATB = 0x07;
                switch (PORTB) {
                    case 0xD7 :
                        recorrer_valores();
                        clave_2 = 0;
                        mostrar_mensaje("0");
                        break;
                    case 0x77 :
                        bandera_clave = 1;
                        break;
                  };
                 __delay_ms(50);
                
                 if ((clave_0 == clave_0_real) & (clave_1 == clave_1_real) & (clave_2 == clave_2_real)){
                     clave_correcta = 1;
                    limpiar();
                    for (int i = 0; i < 3; i++) {
                        LATEbits.LATE0 = 1;
                        __delay_ms(200);
                        LATEbits.LATE0 = 0;
                        LATEbits.LATE1 = 1;
                        __delay_ms(200);
                        LATEbits.LATE1 = 0;
                        LATEbits.LATE2 = 1;
                        __delay_ms(200);
                        LATEbits.LATE2 = 0;
                        __delay_ms(200);
                    }
            }
                 else{
                    clave_correcta = 0; 
                 }
                 
                 
            }
            bandera_clave = 0;
             
                
        }  
    };
    
    return;
}
