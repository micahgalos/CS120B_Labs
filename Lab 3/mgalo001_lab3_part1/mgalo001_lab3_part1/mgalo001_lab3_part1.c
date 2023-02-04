/*
 * mgalo001_lab3_part1.c
 *
 * Created: 4/8/2019 11:19:22 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 1
 * Exercise Description: Count the number of 1s on ports A and B and output that number on port C.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Header
#include <avr/io.h>

// Function Accessors: Bit-access function
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, set to 0
	
	unsigned char tmpA = 0x00; // Initialize PA to 0
	unsigned char tmpB = 0x00; // Initialize PB to 0
	unsigned char count = 0; // Initialize count to 0;
	
	// undefined iterator i 
	unsigned char i;
	
    while (1) 
    {
		tmpA = PINA; // Reads PA 
		tmpB = PINB; // Reads PB
		
		for(i = 0; i < 8; i++){ // counts through every bit that was hit until it reaches the last
			count += GetBit(tmpA, i) + GetBit(tmpB, i);
			}
	PORTC = count; // Outputs the number of bits hit from count
	count = 0; // Reset the count for safety measures
    }
}

