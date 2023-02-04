/*
 * mgalo001_lab3_part2.c
 *
 * Created: 4/8/2019 11:53:25 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 2
 * Exercise Description: A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). A series of
						 LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2,
						 PC5 lights. If the level is 3 or 4, PC5 and PC4 light. Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2.
						 10-12 lights PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should
						 light if the level is 4 or less.  (The example below shows the display for a fuel level of 3).  
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Header
#include <avr/io.h>

// Function Accessors: Bit-access function
/*unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}*/

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, set to 0
	
	unsigned char tmpA = 0x00; // Initialize PA to 0 (fuel level)
	unsigned char tmpC = 0x00; // Initialize PC to 0 (sensor)
	
	while (1)
	{
 		tmpA = PINA & 0x0F; // Reads the fuel level
			
		if(tmpA == 1 || tmpA == 2){ // Fuel level 1 or 2, tmpC is PC5 [0x20]
			tmpC = 0x20;
		}
		if(tmpA == 3 || tmpA == 4){ // Fuel level 3 or 4, tmpC is PC5-PC4 [0x30]
			tmpC = 0x30;
		}
		if(tmpA == 5 || tmpA == 6){ // Fuel level 5 or 6, tmpC is PC5-PC3 [0x38]
			tmpC = 0x38;
		}
		if(tmpA >= 7 && tmpA <= 9){ // Fuel level 7 to 9, tmpC is PC5-PC2 [0x3C]
			tmpC = 0x3C;
		}						
		if(tmpA >= 10 && tmpA <= 12){ // Fuel level 10 to 12, tmpC is PC5-PC1 [0x3E]
			tmpC = 0x3E;
		}
		if(tmpA >= 13 && tmpA <= 15){ // Fuel level 13 to 15, tmpC is PC5-PC0 [0x3F]
			tmpC = 0x3F;
		}
		if(tmpA < 4){ // Fuel level 4 or less, tmpC is PA6 [0x40]
			tmpC += 0x40;
		}				
		PORTC = tmpC; // Outputs the resulting sensor from fuel level
		tmpC = 0x00; // Resets the value from last result
	}
}

