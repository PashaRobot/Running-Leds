/*
 * Leds1.cpp
 *
 * Created: 26.09.2021 12:45:01
 * Author : Pavel Bezdelev
 */ 

#include <avr/io.h>
#include <util/delay.h>

void move(uint32_t leds)
{
	PORTA_OUT = leds;
	PORTB_OUT = leds >> 8;
	PORTC_OUT = leds >> 12;
	_delay_ms(200);
}

int main(void)
{
	PORTA_DIR = 0xFF;
	PORTB_DIR = 0x0F;
	PORTC_DIR = 0xFF;
	uint32_t led = 1;
	uint32_t dir = 1;
	uint32_t maxbit = 1;
	maxbit <<= 19;

    while (1) 
    {
		move(led);
		
		if(led < maxbit && dir)
		{
			led <<= 1;
			if(led == maxbit) dir = 0;
		}
		else if(led > 1 && !dir)
		{
			led >>= 1;
		}
		else
		{
			dir = 1;
		}
    }
}

