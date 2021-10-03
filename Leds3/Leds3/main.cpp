/*
 * Leds3.cpp
 *
 * Created: 17.09.2021 11:11:22
 * Author : Павел
 */ 

#include <avr/io.h>
#include <util/delay.h>

void ShowLEDs(uint32_t Leds){
	PORTA_OUT = Leds 	& 0xFF;
	PORTB_OUT = Leds >> 8   & 0x0F;
	PORTC_OUT = Leds >> 12  & 0xFF;
	_delay_ms(120);
}
int main(void)
{
	volatile uint32_t MaxBit  = 1;
	MaxBit = MaxBit << 19;
	uint32_t RunningLed = 1;
	uint8_t Direction = 0;

	PORTA_DIR = 0xFF;
	PORTB_DIR = 0x0F;
	PORTC_DIR = 0xFF;
	
	while(1)
	{
		ShowLEDs(RunningLed);
		
		if(Direction == 0)                    // Если напрвление справа налево (число бегущих битов увеличивается)
		{
			if (RunningLed & MaxBit)          // Если бегущий бит совпадает с максимальным 
			{
				RunningLed = RunningLed >> 1; // Сдвигаем бит
				RunningLed |= MaxBit;         // 01000000 ИЛИ 10000000 = 11000000 = RunningLed и тд ...
				Direction = 1;                // Меняем направление
			} else
			RunningLed = RunningLed << 1;     // Иначе двигаем биты справа налево
			
		} else if (Direction == 1)            // Если направление слева направо (число бегущих битов увеличивается)
		{
			if (RunningLed & 1)               // Если бегущий бит совпадет с минимальным
			{
				RunningLed = RunningLed << 1; // Сдвигаем бит
				RunningLed |= 1;              // 00000010 ИЛИ 00000001 = 00000011 = RunningLed и тд ...
				Direction = 0;                // Меняем направление
			} else
			RunningLed = RunningLed >> 1;     // Иначе двигаем биты слева направо
		}
		
		/* На этом этапе биты заполнили все ячейки */
		
		else if ( (Direction & 3) == 2)       // Иначе если направление справа налево (число бегущих битов уменьшается)
		{
			RunningLed = RunningLed << 1;     // Двигаем бит налево
			RunningLed &= (MaxBit << 1) - 1;  // Отсекаем самый левый бит: 111111110 И 011111111 = 011111110
			// Меняем направления
			if (Direction & 4)                
			Direction = 3;
			else if (RunningLed & MaxBit)
			Direction |= 4;
			
		}
		else if ( (Direction & 3) == 3)   // Иначе если направление слева направо (число бегущих битов уменьшается)
		{
			RunningLed = RunningLed >> 1; // Двигаем бит направо
			// Меняем направления
			if (Direction & 4)
			Direction = 2;
			else if (RunningLed & 1)
			Direction |= 4;
		}
		
		if(RunningLed == (MaxBit << 1) - 1)  // Если все биты единицы
		Direction |= 2 | 4;
		else if ((RunningLed == MaxBit) && (Direction & 3) == 2) // Если остался один бит
		Direction = 0; // Начинаем все сначала
	}
}