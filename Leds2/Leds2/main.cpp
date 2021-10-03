/*
 * Leds2.cpp
 *
 * Created: 26.09.2021 13:40:05
 * Author : Pavel Bezdelev
 */ 

#include <avr/io.h>
#include <util/delay.h>

void move(uint32_t leds)
{
	PORTA_OUT = leds;
	PORTB_OUT = leds >> 8;
	PORTC_OUT = leds >> 12;
	_delay_ms(150);
}

int main(void)
{
	PORTA_DIR = 0xFF;
	PORTB_DIR = 0x0F;
	PORTC_DIR = 0xFF;
	uint32_t led = 1;
	uint32_t dir = 1;
	uint32_t minbit = 1;
	uint32_t maxbit = 1;
	uint32_t StableLed = 0;
	maxbit <<= 19;

	while (1)
	{
		move(led | StableLed);
		
		if((led < maxbit) && (dir) && (StableLed != (1 << 20) - 1)) 
		{
			led <<= 1;
			if(led == maxbit) 
			{
				StableLed |= led;
				maxbit >>= 1;
				dir = 0;
			}
		}
		else if((led > minbit) && (!dir) && (StableLed != (1 << 20) - 1))
		{
			led >>= 1;
			if(led == minbit)
			{
				StableLed |= led;
				minbit <<= 1;
				dir = 1;
			}
		}
		else 
		{
			led = 1;
			dir = 1;
			minbit = 1;
			maxbit = 1;
			StableLed = 0;
			maxbit <<= 19;
		}
	}
}