/*
 * mgalo001_lab3_part5.c
 *
 * Created: 4/9/2019 9:26:18 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 5
 * Exercise Description: (Challenge): A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511) and
 *						 connects to input PD7..PD0PB0 on the microcontroller. If the weight is equal to or above 70 pounds, 
 *						 the airbag should be enabled by setting PB1 to 1. If the weight is above 5 but below 70, the airbag 
 *						 should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1. (Neither B1 nor B2 
 *						 should be set if the weight is 5 or less, as there is no passenger).
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Header
#include <avr/io.h>

// Bit-access functions
/*unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}*/

int main(void)
{
	DDRB = 0xFE; PORTB = 0x01; // Configure Port B's 8 pins as Outputs, set to 1 for bit 0 exclusively
	DDRD = 0x00; PORTD = 0xFF; // Configure Port D's 8 pins as Inputs
	
	unsigned char tmpB0 = 0x00; // bit flag
	unsigned short tmpB1 = 0x00; // total weight
	unsigned char tmpD = 0x00; // passenger weight
	 
	// Start Execution Here
	while (1)
	{
		tmpD = PIND;
		tmpB0 = PINB & 0x01;
		
		tmpB1 = tmpD + (tmpB0 * 255);
		
		if(tmpD >= 70){
			tmpB1 = (tmpB1 & 0x01) | 0x01 << 2;
			
		}
		else if(tmpD >= 5){
			tmpB1 = ((tmpB1 & 0x01) | 0x01) << 1;
		}
		else{
			tmpB1 = 0x00;
		}
		
		PORTB = tmpB1;
		tmpB1 = 0x00;
	}
}
