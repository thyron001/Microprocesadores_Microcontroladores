LIST p=18f4550
	#include "p18f4550.inc" 
	
	;Definicion de vectores:
	;-------------------------------------------------------
	org 0  ;vector de reset
	goto Inicio
	;-------------------------------------------------------

	;Declaracion de variables
	;-------------------------------------------------------
	Contador 	equ 0x30
	Contador2 	equ 0x31
	Contador3 	equ 0x32
	Contador4 	equ 0x33
	veces		equ 0x34
	Contador_led   equ 0x35
	;-------------------------------------------------------

	org 20
	Inicio
	
	;-------------------------------------------------------
	
	;Inicializacion de variables

	movlw	d'38'		
	movwf 	Contador 		;inicio contador=38
	movlw	d'9'
	movwf 	Contador2 		;inicio contador2=9
	movlw	d'10'
	movwf 	Contador3 		;inicio contador3=10
	movlw	d'11'
	movwf	veces			;Veces que se enciende LED_inicio
	movlw	d'7'
	movwf	Contador_led	;Cantidad de luces en secuencia
	;-------------------------------------------------------

	;Configuracion de perifericos

	;Se definen puertos de entrada y salida
	CLRF	TRISB 		;Puerto B salida
	CLRF    TRISD		;Puerto D salida
	SETF    TRISC		;Puerto C entrada

	;Condiciones iniciales
	CLRF	LATB		;Puerto B empiece apagado
	CLRF    LATD		;Puerto D empiece apagado	

	;-------------------------------------------------------

	;Parpadeo del LED piloto

	LED_inicio:	
		bsf   	LATD, 0
		movlw	d'250'		;X=50
		call 	Delay_ms	;delay generico para X ms
		bcf   	LATD, 0 	;enciende el LED en RB0;
		movlw	d'250'		;X=50
		call 	Delay_ms	;delay generico para X ms
		decfsz	veces, 1
		goto 	LED_inicio
		bsf   	LATD, 0
 	;-------------------------------------------------------

	;Consulta de boton de inicio

	consulta_1:                 ;Escucha infinitamente el boton S2  
		movlw   0x00        
		movwf   LATB        	;Apaga todos los LEDs del puerto B
		btfsc 	PORTC,0     	;Si RC0 (S2) esta en bajo se salta la siguiente linea
		goto 	consulta_1  	;Si RC0 (S2) no esta en bajo, se repite la escucha
	
	    ;Se espera un tiempo muy corto para asegurarse que el boton esta pulsado
	    ;para evitar efectos de rebote

	    movlw	d'50'       
		call 	Delay_ms
	
		btfsc 	PORTC, 0    
		goto 	consulta_1	
	
	secuencia:                  ;Inicializacion de valores de la secuencia
		movlw   d'7'	    	;Variable para saber cuantos desplazamientos debe hacer el byte
		movwf   Contador_led   	;Contador para saber en que posicion de LED esta la secuencia
		movlw   0x01           
		movwf   LATB           	;Se inicializa la secuencia con un bit encendido en el primer LED       
		goto 	mover_izquierda ;Se inicia la secuencia desde derecha a izquierda
	
	mover_izquierda:
		movlw	d'250'	
		call 	Delay_ms		;delay generico de 250ms
	
		btfss 	PORTC,1     	;Si RC1 (S1) esta en alto (no pulsado) se salta la siguiente linea
		goto 	consulta_1  	;Si RC1 (S1) no esta en alto, se vuelve al inicio del programa
	
	    ;Se espera un tiempo muy corto para asegurarse que el boton esta pulsado
	    ;para evitar efectos de rebote
		movlw	d'50'       
		call 	Delay_ms
	
		btfss 	PORTC,1
		goto 	consulta_1
	
		rlcf    LATB, 1        	;Si no esta pulsado, se desplaza un bit hacia la izquierda el byte
		decfsz	Contador_led, 1 ;Se decrementa el contador que cuenta el LED actual de la secuencia, si llega a 0, se salta la siguiente linea
		goto 	mover_izquierda	;Se repite el movimiento hacia la izquierda
	
		movlw   d'7'       		;Si todos los 8 LEDs se encendieron, se empieza el desplazamiento hacia la derecha
		movwf   Contador_led
		goto    mover_derecha	
	
	mover_derecha:
		movlw	d'250'		
		call 	Delay_ms		
	
		btfss 	PORTC,1
		goto 	consulta_1
	
		movlw	d'50'
		call 	Delay_ms
	
		btfss 	PORTC,1
		goto 	consulta_1	
	
		rrcf    LATB, 1
		decfsz	Contador_led, 1
		goto 	mover_derecha
	
		movlw   d'7'	
		movwf   Contador_led
		goto    mover_izquierda   

	;-----------------------------------------------

	;Delay de 10 us	

	Delay_10us:	
		decfsz Contador,1  ; contador  = contador-1
		goto Delay_10us
		movlw d'38'
		movwf Contador ;reinicio del contador
		return
	;--------------------------------------------------------------
	
	;Delay de 100 us

	Delay_100us:
		call Delay_10us
		decfsz Contador2,1  ; contador2  = contador2-1
		goto Delay_100us
		movlw d'9'
		movwf Contador2 ;reinicio del contador2
		return
	;--------------------------------------------------------------
	
	;Delay de 1ms

	Delay_1ms:
		call Delay_100us
		decfsz Contador3,1  ; contador3 = contador3-1
		goto Delay_1ms
		movlw d'10'
		movwf Contador3 ;reinicio del contador3
		return
	
	;--------------------------------------------------------------

	;Delay que recibe el dato en W para generar cualquier delay en ms

	Delay_ms:
		;Asume que en W	 tiene almacenado  el # de ms
		movwf Contador4  ;Contador4 = W (acumulador)
	repite:	
		call Delay_1ms
		decfsz Contador4,1
		goto repite
		return
	;--------------------------------------------------------------
	
end