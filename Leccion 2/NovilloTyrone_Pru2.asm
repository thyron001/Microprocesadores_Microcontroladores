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
	movlw	d'10'
	movwf	veces			;Veces que se enciende LED_inicio
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
		movlw	d'200'		;X=50
		call 	Delay_ms	;delay generico para X ms
		bcf   	LATD, 0 	;enciende el LED en RB0;
		movlw	d'200'		;X=50
		call 	Delay_ms	;delay generico para X ms
		decfsz	veces, 1
		goto 	LED_inicio
		bsf   	LATD, 0	
 	;-------------------------------------------------------

	;Consulta de boton de inicio

	consulta_1:                 ;Escucha infinitamente el boton S2  
		call 	apagar_pines
		btfsc 	PORTC,0     	;Si RC0 (S2) esta en bajo se salta la siguiente linea
		goto 	consulta_1  	;Si RC0 (S2) no esta en bajo, se repite la escucha
	
	    ;Se espera un tiempo muy corto para asegurarse que el boton esta pulsado
	    ;para evitar efectos de rebote

	    movlw	d'50'       
		call 	Delay_ms
	
		btfsc 	PORTC, 0    
		goto 	consulta_1

	
	secuencia:  
	
		;Numero 1
		call	apagar_pines
		bsf		LATB, 1
		bsf		LATB, 2

		btfss 	PORTC,1
		goto 	consulta_1				
	
		movlw	d'500'	
		call 	Delay_ms
 		
		;Numero 2	
		call	apagar_pines
		bsf		LATB, 0
		bsf		LATB, 1
		bsf		LATD, 6
		bsf		LATD, 4
		bsf		LATB, 3

		btfss 	PORTC,1
		goto 	consulta_1	

		movlw	d'500'	
		call 	Delay_ms
		
		;Numero 3
		call	apagar_pines
		bsf		LATB, 0
		bsf		LATB, 1
		bsf		LATD, 6
		bsf		LATB, 2
		bsf		LATB, 3

		btfss 	PORTC,1
		goto 	consulta_1	

		movlw	d'500'	
		call 	Delay_ms

		;Numero 4
		call	apagar_pines
		bsf		LATD, 5
		bsf		LATD, 6
		bsf		LATB, 1
		bsf		LATB, 2

		btfss 	PORTC,1
		goto 	consulta_1	

		movlw	d'500'	
		call 	Delay_ms

		;Numero 5
		call	apagar_pines
		bsf		LATB, 0
		bsf		LATD, 5
		bsf		LATD, 6
		bsf		LATB, 2
		bsf		LATB, 3

		btfss 	PORTC,1
		goto 	consulta_1

		movlw	d'500'	
		call 	Delay_ms
		
		call 	apagar_pines

		movlw	d'1000'	
		call 	Delay_ms

		goto	secuencia


apagar_pines:
		bcf		LATB, 0
		bcf		LATB, 1
		bcf		LATB, 2
		bcf		LATB, 3
		bcf		LATD, 4
		bcf		LATD, 5
		bcf		LATD, 6
		return

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