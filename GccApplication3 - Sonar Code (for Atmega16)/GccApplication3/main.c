/*
 * GccApplication6.c
 *
 * Created: 26-Aug-19 8:54:11 AM
 * Author : acer
 */ 

#include <avr/io.h>
#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

static volatile int pulse1 = 0;
static volatile int i1 = 0;
/*static volatile int pulse2 = 0;
static volatile int i2 = 0;*/

ISR(INT0_vect)
{
	//timer read
	if(i1 == 1)
	{
		pulse1 = TCNT1;
		TCCR1B = 0;
		TCNT1 = 0;
		i1 = 0;
	}
	//timer start
	else if(i1 == 0)
	{
		//Anisha added (should be wrong)
		TCNT1 = 0;
		TCCR1B |= (1<<CS10);
		i1 = 1;
	}
}

/*
ISR(INT1_vect) {
	//timer read
	if(i2 == 1)
	{
		pulse2 = TCNT1;
		TCCR1B = 0;
		TCNT1 = 0;
		i2 = 0;
	}
	else if(i2 == 0)
	{
		TCNT1 = 0;
		TCCR1B |= (1<<CS10);
		i2 = 1;
	}
}*/


int main(void)
{
	DDRA = 0b11111111;
	
	PORTA = 0x00;
	/* Replace with your application code */
	
	uint32_t count_a = 0;
	//uint32_t count_b = 0;
	DDRD = 0b11110011;
	
	GICR |= (1<<INT0) /*| (1<<INT1)*/;
	MCUCR |= (1<<ISC00) /*| (1<<ISC10)*/;
	
	TCCR1A = 0;
	sei();
	
    while (1)
    {
		PORTD |= (1<<PIND0)/* | (1<<PIND1)*/;
	    _delay_us(15);
		PORTD &= ~(1<<PIND0)/* & ~(1<<PIND1)*/;
		_delay_ms(100);
		count_a = pulse1/58;
		//count_b = pulse2/58;
		
		if(count_a <= 30/* || count_b <= 30*/)
		{
			count_a = 0;
			PORTA = 0b00000001;
		}
	    else
		{
			PORTA = 0b00000000;
		}
		_delay_ms(100);
    }
}

