/*
 * File:   lcd_confi.c
 * Author: javier Sotamba
 *
 * Created on 23 de julio de 2024, 10:15
 */


#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcd_confi.h"

#define _XTAL_FREQ 48000000 // frecuencia de oscilacion 

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


//Transmitir un byte DATO al LCD
//Dato = 0xWZ
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

void LCD_Data(unsigned char data) {
    LATDbits.LD1 = 1;
    txbits(data, 1, 1);
    LATDbits.LD1 = 1;
}