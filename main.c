#include <xc.h>
#include <plib.h>

int bandera;

void delay_5ms(char veces){
    for( char j = 0; j<veces; j++){
        for(int i = 0; i < 50000; i++);
    }
}

void __ISR(_TIMER_1_VECTOR, IPL1SOFT) Timer1IntRoutine(void){
    LATGbits.LATG6 = ~PORTGbits.RG6;
    IFS0bits.T1IF = 0;
}

void secuencia_leds(void){
    for( char j = 0; j<10; j++){
        LATE = 0x00;
        delay_5ms(1);
        LATE = 0xFF;
        delay_5ms(1);
    }    
}

void main(void) {
    
    SYSTEMConfigPerformance(80000000L);    
    
    //configuración de periféricos***
    
    //CONFIGURACIPON DE E/S
    TRISFbits.TRISF0 = 0; //RF0 SALIDA LD5
    TRISGbits.TRISG6 = 0;
    TRISE = 0x00;
    TRISFbits.TRISF1 = 1;
    
    //condiciones iniciales
    LATFbits.LATF1 = 0; //empieza apagado
    LATE = 0x00;
    //************
    
    secuencia_leds();
    LATE = 0x00;
    
    while (1){        
        if (PORTFbits.RF1 == 1 && bandera == 0){
            bandera = 1;
        }
        if (PORTFbits.RF1 == 1 && bandera == 1){
            bandera = 0;
        }
        if (bandera == 1){
            secuencia_leds();
        }
        if (bandera == 0){
            LATE = 0x00;
        }      
    }       
    return;
}