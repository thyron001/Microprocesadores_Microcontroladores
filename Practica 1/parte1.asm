;PROGRAMA PIC 18F4550
;Tyrone Novillo
;9 de Abril, 2024

;----------------------------------------------

	LIST p=18f4550  ;especificar dispositivo
	#include "p18f4550.inc"
	
	;Definicion de vectores:
	;-------------------------------------
	org 0		;vector de reset	
	goto Inicio
	;-------------------------------------
	
	;DECLARACION DE VARIABLES:
	;-------------------------------------	
	Contador 	equ 0x30
	Contador2 	equ 0x31
	Contador3 	equ 0x32
	Contador4 	equ 0x33
	veces		equ 0x34
	Contador_led   equ 0x35
	;-------------------------------------
	
	org 20
Inicio

	;INICIALIZACION DE VARIABLES:
	;-------------------------------------	
	movlw	d'38'		
	movwf 	Contador ;inicio contador=38
	movlw	d'9'
	movwf 	Contador2 ;inicio contador2=9
	movlw	d'10'
	movwf 	Contador3 ;inicio contador3=10
	movlw	d'11'
	movwf	veces	;parpadeo
	movlw	d'1'
	movwf	Contador_led	;parpadeo
	;-------------------------------------

	
	;**************************************
	;CONFIGURACI?N DE PERIFERICOS *********

	;PUERTOS DE E/S	
	;PORTB: RB0 es salida
	CLRF	TRISB ;Definir si el puerto es in o out
	BCF     TRISD, 0	

	;CONDICIONES INICIALES E/S
	CLRF	LATB	;PORTB empiece apagado
	BCF     LATD, 0
	
	;**************************************

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

	;BUCLE PRINCIPAL
	;-------------------------------------
main:
	movlw   0xAA
	movwf   LATB;enciende el LED en RB0\
	movlw	d'250'		;X=50
	call 	Delay_ms	;delay generico para X ms
	movlw   0x55
	movwf   LATB		;enciende el LED en RB0;
	movlw	d'250'		;X=50
	call 	Delay_ms	;delay generico para X ms
	goto main	
;-------------------------------------	
;Delay de 10 us ------------------------------
Delay_10us:	
	decfsz	Contador,1	;contador=contador-1
	goto 	Delay_10us
	movlw	d'38'
	movwf	Contador	;reinicio del contador
	return
;---------------------------------------------


;Delay de 100us ------------------------------
Delay_100us:
	call	Delay_10us
	DECFSZ	Contador2,1
	goto 	Delay_100us
	movlw	d'29'
	movwf	Contador2	;reinicio del contador2
Exit		
	decfsz	Contador2,1
	goto 	Exit
	movlw	b'1001' 		;9
	movwf	Contador2	;reinicio del contador2	
	return	
;---------------------------------------------
	
	
;Delay de 1ms --------------------------------
Delay_1ms:
	call 	Delay_100us
	decfsz	Contador3,1
	goto 	Delay_1ms
	movlw	d'10'
	movwf	Contador3
	return	
;---------------------------------------------


;---------------------------------------------
;Delay que recibe el dato en W para generar 
;cualquier delay en ms
Delay_ms:
	;Asume que W tiene almacenado el # de ms
	movwf Contador4	;Contador4 = W
repite:	
	call 	Delay_1ms
	decfsz	Contador4,1
	goto	repite
	return	
;---------------------------------------------	
	

end
	