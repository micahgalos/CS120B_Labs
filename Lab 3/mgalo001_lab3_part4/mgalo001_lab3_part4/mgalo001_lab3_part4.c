/*
 * mgalo001_lab3_part4.c
 *
 * Created: 4/9/2019 9:16:15 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 4
 * Exercise Description: (Challenge): Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. That is to 
						 say,  take the upper nibble of PINA and map it to the lower nibble of PORTB, likewise take the lower
						 nibble of PINA and map it to the upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
*/

// Library Header
#include <avr/io.h>

// Bit-Access Functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 pins as Inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure Port B's 8 pins as Outputs, set to 0s
	DDRC = 0xFF; PORTC = 0x00; // Configure Port C's 8 pins as Outputs, set to 0s

	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
	
	while (1)
	{
		tempB = (PINA & 0xF0) >> 4;	// Upper Nibble of Port A to Lower Nibble of Port B
		tempC = (PINA & 0x0F) << 4;	// Lower Nibble of Port A to Upper Nibble of Port C
		
		PORTB = tempB; // Outputs the shifted nibble of Port B
		PORTC = tempC; // Outputs the shifted nibble of Port C
		tempB = 0x00; // Clears tempB output
		tempC = 0x00; // Clears tempC output
	}
}

