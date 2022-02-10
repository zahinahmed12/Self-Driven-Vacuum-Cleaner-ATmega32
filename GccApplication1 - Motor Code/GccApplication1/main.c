/*
 * GccApplication1.c
 *
 * Created: 24-08-2019 10:20:43 PM
 * Author : AGS
 */ 
#define F_CPU 1000000UL
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "MotorDrive.h"
#include <avr/interrupt.h>
#include "new_UART.h"
static volatile int cnt = 0;
volatile int last = 0;
unsigned char lspeed = 0, rspeed = 0;
unsigned char turnspeed = 0;

ISR(INT0_vect) {
	
		set_speed(0,0);
		_delay_ms(1000);
		
		set_speed(175,70);
		_delay_ms(400);
		set_speed(turnspeed+5,turnspeed);
		_delay_ms(1000);
		set_speed(0,0);
		_delay_ms(500);
		set_speed(rspeed,lspeed);
}

ISR(USART_RXC_vect) {
	unsigned char buffer[16];
	unsigned char ch = UDR;
	if (ch == 'i') {
		MCUCR |= (1<<ISC00)|(1<<ISC01);
		UART_receive_string((const char*) buffer,3);
		turnspeed  = atoi(buffer);
		init_motor_pins();
		UDR = ch;
		UDR = '\n';
	}
	else if (ch == 's') {
		unsigned int x;
		UART_receive_string(buffer,3);
		UART_send_string(buffer);
		x = atoi((const char*) buffer);
		GICR |= (1<<INT0);
		set_speed(x+5, 100+x);
		lspeed = 100+x;
		rspeed = x+5;	
		UDR = 's';
		UDR = '\n';
	}
	else if (ch == 'z') {
		set_speed(0, 0);
		lspeed = 0;
		rspeed = 0;
		GICR &= ~(1<<INT0);
		UDR = ch;
		UDR = '\n';
	}
	else if (ch == 'b') {
		unsigned int x;
		GICR |= (1<<INT0);
		UART_receive_string(buffer,3);
		UART_send_string(buffer);
		x = atoi((const char*) buffer);
		set_speed(105+x,x);
		lspeed = x;
		rspeed = 105+x;
		UDR = 'b';
		UDR = '\n';
	}
	else if (ch == 'l') {
		unsigned int x;
		GICR |= (1<<INT0);
		UART_receive_string(buffer,3);
		UART_send_string(buffer);
		x = atoi((const char*) buffer);
		set_speed(0,x);
		lspeed = x;
		rspeed = 0;
		UDR = 'b';
		UDR = '\n';
	}
	else if (ch == 'r') {
		unsigned int x;
		GICR |= (1<<INT0);
		UART_receive_string(buffer,3);
		UART_send_string(buffer);
		x = atoi((const char*) buffer);
		set_speed(x,0);
		lspeed = 0;
		rspeed = x;
		UDR = 'b';
		UDR = '\n';
	}
}




int main(void)
{

	PORTA = 0x00;
	
	UART_init();
	sei();
    while (1) 
    {
    }
}

