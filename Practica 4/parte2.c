/*
 * File:   test.c
 * Author: Javier Sotamba 
 *
 * Created on 23 de mayo de 2024, 9:17
 */
// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

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
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
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


#define _XTAL_FREQ 48000000 // frecuencia de oscilacion 

unsigned char dis[20] = {0xFD, 0x61 , 0xDB, 0xF3, 0x67, 0xB7, 0x3F, 0xE1, 0xFF, 0xF7, 0xEF, 0x3F, 0x9D, 0x7B, 0x9F, 0x8F, 0xBF, 0x1D, 0x3B, 0x0D }; 
const char clave[4] = {0x01, 0x02, 0x03, 0x04};
int i=0; 
int con = 0; 
int sel = 0; 
int led_1 = 0;
int led_2 = 0;
int led_3 = 0;
int led_4 = 0;
int led_4_tem = 0;
int led_2_tem = 0;
int led_3_tem = 0;
int encendido_led = 0;

void RD0_Toggle(void){ // Toggel de D0 
    LATDbits.LD0 = ~PORTDbits.RD0;
}

void recorrer_valores(void){
    led_4_tem = led_4;
    led_3_tem = led_3;
    led_2_tem = led_2;
    led_3 = led_4_tem;
	led_2 = led_3_tem;
	led_1 = led_2_tem;
    return;
                
    }

void init(void){ //set up inicial 
     // perifericos configuracion
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 1; 
    //**
    
    // E/S configuracion
    TRISD = 0x00; // segmentos de displays 
    TRISC = 0x30; // activacion de displays y entradas de pulsantes
    TRISB = 0xF0;
   //**

    // Condiciones iniciales
    LATD = 0x00; // display y otros perifericos en off inicial 
    LATC = 0x00; // 
    //****
}
//si s3 entonces esto no tiene que parpadear 
//si s2 entonces esto tiene que hacer una transicion 
void __interrupt() inter(void){
    if (INTCONbits.TMR0IF){
        // Aqui se prende cada display.
        // Limpia la bandera de interrupción
        INTCONbits.TMR0IF = 0;
        TMR0L = 0x2E; //inicializa el conteo 
        if (con == 0){
            LATC = 0x80; //Control
            LATD = dis[led_1] ; //Display 
            con++; 
        
        }else if(con == 1){
            LATC = 0x40; //Control
            LATD = dis[led_2]; //Display 
            con++; 
            
        }else if (con == 2){
            LATC = 0x02; //Control
            LATD = dis[led_3]; //Display 
            con++;
              
        }else{
            LATC = 0x01; //Control
            LATD = dis[led_4]; //Display 
            con = 0; 
        }
        
        LATDbits.LD0 = encendido_led;

    }          
    
}

// Programa principal

void main() {
    
    init();
    
    //Subrutina de inicialiacion
    //char es un tipo de datos de 8 bits
    for (char conteo = 0; conteo < 19 ; conteo++){
        RD0_Toggle();
        __delay_ms(250);
    }
    
    encendido_led = 1;
    
    
    // Configuración del TMR0 
    INTCONbits.GIE = 1; // habilta interrupciones globales 
    T0CONbits.T08BIT = 1; // Modo de 8 bits
    T0CONbits.T0CS = 0; // Fuente de reloj interna
    T0CONbits.PSA = 0; // Utiliza el prescaler
    T0CONbits.T0PS = 0b111; // Prescaler 1:256

    // Valor inicial del TMR0 
    TMR0L = 0x2E; // para conteo de 1 ms 

    // Habilita la interrupción por desbordamiento
    INTCONbits.TMR0IE = 1;

    // Inicia el temporizador
    
    
    T0CONbits.TMR0ON = 1; //iniciamos el temporizador 
    
    
    // main loop, el LED en RD0 permanece encendido 
    while (1) {
        LATB = 0x0E;
        switch (PORTB) {
            case 0xEE:
                recorrer_valores();
                led_4 = 1;
                break;
            case 0xDE :
                recorrer_valores();
                led_4 = 2;
                break;
            case 0x7E :
                recorrer_valores();
                led_4 = 3;
                break;
          };
         __delay_ms(50);
         
        LATB = 0x0D;
        switch (PORTB) {
            case 0xED:
                recorrer_valores();
                led_4 = 4;
                break;
            case 0xDD :
                recorrer_valores();
                led_4 = 5;
                break;
            case 0x7D :
                recorrer_valores();
                led_4 = 6;
                break;
          };
         __delay_ms(50);
         
         LATB = 0x0B;
        switch (PORTB) {
            case 0xEB:
                recorrer_valores();
                led_4 = 7;
                break;
            case 0xDB :
                recorrer_valores();
                led_4 = 8;
                break;
            case 0xBB :
                led_1 = 0;
                led_2 = 0;
                led_3 = 0;
                led_4 = 0;
                break;
            case 0x7B :
                recorrer_valores();
                led_4 = 9;
                break;
          };
         __delay_ms(50);
         
         
          LATB = 0x07;
        switch (PORTB) {
            case 0xD7 :
                recorrer_valores();
                led_4 = 0;
                break;
            case 0xB7 :
                if (led_1 == clave[0] && led_2 == clave[1] && led_3 == clave[2] && led_4 == clave[3]){
                    led_1 = 16;
                    led_2 = 18;
                    led_3 = 18;
                    led_4 = 13;
                    for (i = 0; i < 3; i++) {
                        encendido_led = 0;
                        __delay_ms(200);
                        encendido_led = 1;
                        __delay_ms(200);
                    }
                }else{
                    led_1 = 15;
                    led_2 = 10;
                    led_3 = 19;
                    led_4 = 17;
                    for (i = 0; i < 10; i++) {
                        encendido_led = 0;
                        __delay_ms(50);
                        encendido_led = 1;
                        __delay_ms(50);
                    }
                }
                break;
          };
         __delay_ms(50);
         
}
    return;
    }
  