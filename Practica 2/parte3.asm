;**Encabezado*********
;Practica2_Parte2_Micriprocesadores
;Juan Jose Duran Chica
;Stalyn Antonio Ochoa Yánez
;Universidad de Cuenca
;***************

;**Especificar Dispositivo****
LIST p=18f4550
#include "p18f4550.inc"
;***************

;***Definicion de Vecotres******
org 0;Vector de Reset
goto Inicio	;Envía a la etiqueta inicio

org 8
goto interrupcion
;***************

;***Declaración de Variables****
	;Espacios de memoeria para cada variable para el Delay General
	Cont100 equ 0x30
	AjusteCont100 equ 0x31 
	Cont1ms	equ	0x32
	ContGen	equ	0x33
	;Espacio de memoria para la variable del Led Piloto
	piloto	equ 0x34
	;espacios de memoria para las secuencias y control de boton
	sec_d equ 0x35
	sec_i equ 0x36
	posLed equ 0x37
	apagar equ 0x38
	parpadeo equ 0x39
;***************
	org 20	;Vector del programa
Inicio

;*Configuracion de perifericos****
	;Se definen los puertos de entrada y salida
	;TRIS se usa para definir si un puerto es entrada o salida
	CLRF TRISB	;Con CLRF se da el valor de 0 a todo B 
				;asigannado a todo el puerto B como salida
	BSF	TRISC,0	;Se ponen en 1 el TRISC del puerto 0 de C 
				;para que sea entrada
	BSF TRISC,1 ;Puerto 1 de C como entrada
	BCF TRISD,0	;Puerto 0 de D como salida

	;Se definen las condiciones iniciales de E/S
	;LAT se usa para esciribir en le puerto 
	;PORT se usa para leer el puerto
	CLRF LATB ;Empieza apagado todo el puerto B
	BCF LATD,0	;Empieza apagado el puerto 0 de D

;****************

;***Inicialización de Variables****
	;Valores de las variables para el Delay
	movlw	d'255' ; se escribe X valor en el acumulador
	movwf	Cont100 ;se escibe el valor del W en la variable
	movlw	d'142'
	movwf	AjusteCont100
	movlw	d'10'
	movwf	Cont1ms
	;Valor de repeticion del encendido del eld piloto
	movlw	d'9'
	movwf	piloto
	movlw d'40'
	movwf apagar
	movlw d'100'
	movwf parpadeo
;****************

;TIMER0----------------------------------------------
	bcf T0CON, TMR0ON 
	bcf T0CON,T08BIT  ; 16bits
	bcf T0CON,T0CS
	bcf T0CON,PSA
	bsf T0CON,T0PS2
	bsf T0CON,T0PS1
	bcf T0CON,T0PS0
;----------------------------------------------------

;TEMPORIZADOR TMR2 ********************
	bcf		T2CON,TMR2ON	;TMR2 empieza APAGADO
	BSF		T2CON,T2CKPS1	;PRESCALER
	BCF		T2CON,T2CKPS0	;1:1
	BSF		T2CON,T2OUTPS3	;POSTSCALER
	BSF		T2CON,T2OUTPS2	;1:3
	BSF		T2CON,T2OUTPS1	;
	BSF		T2CON,T2OUTPS0	;
	
	movlw	d'234'	;PR2=234 para
	movwf	PR2		;contar 5ms
	;**************************************

;configuracion de perofericos------------------------
;timer0
	movlw d'72'
	movwf TMR0H; 
	movlw d'229'
	movwf TMR0L;  ;desde el cual empieza a contar

	bsf T2CON,TMR2ON 
;----------------------------------------------------


;configuracion de interruociones---------------------
	bsf INTCON, GIE
	BSF INTCON, TMR0IE
	
	bsf INTCON,PEIE
	BSF PIE1,TMR2IE
;----------------------------------------------------


;*BUCLE PRINCIPAL********
;Encendido/Apagado LED Piloto
led_piloto:
	bsf	LATD,0	;escirbo 1 en puerto 0 de D
	movlw d'50'	;escribe 50 decimal en el acumulador
				;para tener un delay de 50 ms
	call Delay_Gen	;llamo a la funcion de delay
	bcf LATD,0	;escribo 0 en el puerto 0 de D
	movlw d'50'
	call Delay_Gen
	decfsz	piloto,1 ;Contador de decremento
	goto led_piloto
	;bsf	LATD,0
;***************

;***Verificacion de pulsacion de botones
testeo:
	btfsc PORTC,0 ;Verifico si el puerto 0 de C es 0 o 1
	goto testeo
	movlw d'50'
	call Delay_Gen
	btfsc PORTC,0 ;Verifico si el puerto 0 de C es 0 o 1
	goto testeo
	bsf T0CON, TMR0ON ;prendemos timer0
	;***************

;**Programacion de la secuencia 1
secuencia1:
	movlw d'7'
	movwf sec_d
	movwf sec_i
	movlw 0x1
	movwf posLed
;******************
;**Programacion del movimiento del LED a la derecha
derecha:
	movff posLed, LATB ;Con movff muevo lo de un espacio de memoria a otro
	movlw d'100'
	call Delay_Gen
	btfss PORTC,1 ;Si el puerto 1 de C es 0 se va a apagado de no salta la ins
	call pausa
	RLNCF posLed,1 ;Se desplaza en 1 el valor de posLed a la derecha
				   ;y se escribe en posLed el nuevo valor
	decfsz sec_d,1
	goto derecha
	movlw 0x80
	movwf posLed
;******************
;**Programacion del movimiento del LED a la izquierda
izquierda:
	movff posLed, LATB
	movlw d'100'
	RRNCF posLed,1
	call Delay_Gen
	btfss PORTC,1
	call pausa
	decfsz sec_i,1
	goto izquierda
	goto secuencia1
;******************

;**Programacion de la pausa
pausa:
	btfsc PORTC,0 ;Verifico si el puerto 0 de C es 0 o 1
	goto pausa
	movlw d'50'
	call Delay_Gen
	btfsc PORTC,0 ;Verifico si el puerto 0 de C es 0 o 1
	goto pausa
	return
;*******************
	
;SUBRUTINA DE INTERRUPCION--------------------------------
interrupcion
	btfss	INTCON,TMR0IF	;Fue el TMR0?
	goto	next
	goto 	TMR0IT

next
	btfss	PIR1,TMR2IF	 	;Fue el TMR2?
	retfie
	goto	TMR2IT

TMR0IT
	;atencion al timer0
	movlw d'72'
	movwf TMR0H		;timer0 no empize en 0
	movlw d'229'
	movwf TMR0L	
	bcf INTCON, TMR0IF  ; blanqueamos la bandera
	decfsz apagar
	retfie
	goto apagado

TMR2IT
	bcf PIR1, TMR2IF  ; blanqueamos la bandera
	decfsz parpadeo
	retfie
	goto ledd
		
ledd
	btg LATD,0  ;niega rb1
	movlw d'100'
	movwf parpadeo
	retfie

apagado
	clrf LATB
	movlw d'40'
	movwf apagar
	btfsc PORTC,0
	goto apagado
	movlw d'7'
	movwf sec_d
	movwf sec_i
	movlw 0x80
	movwf posLed
	retfie


;--------------------------------------------------------

;**Programacion del Delay de uso General***
Delay_100us:
	decfsz Cont100,1
	goto Delay_100us
	movlw d'255'
	movwf Cont100
	goto Ajuste100
	Ajuste100:
		decfsz AjusteCont100,1
		goto Ajuste100
		movlw d'142'
		movwf AjusteCont100
		return

Delay_1ms:
	call Delay_100us
	decfsz Cont1ms,1
	goto Delay_1ms
	movlw d'10'
	movwf Cont1ms
	return

Delay_Gen:
	movwf ContGen
	repite:
		call Delay_1ms
		decfsz ContGen,1
		goto repite
		return
end