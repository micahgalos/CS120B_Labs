/*
 * mgalo001_lab2_part0.c
 *
 * Created: 4/3/2019 12:10:45 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail:
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 0
 * Exercise Description: Prelab
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

// Library Header
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as outputs
	DDRB = 0xFF; PORTB = 0x00;	// Configure port B's 8 pins as outputs
								// Initialize PORTB outputs to 0s
	unsigned char tmpB = 0x00;
	unsigned char tmpA = 0x00;
    while (1) 
    {
		// 1) Read Input
		tmpA = PINA & 0x01;
		// 2) Perform computation
		// If PA0 is 1, set PB1PB0 = 01, else = 10
		if(tmpA == 0x01){ // True if PA0 is 1
			tmpB = (tmpB & 0xFC) | 0x01; // Sets tmpB to bbbbbb01
										 // Clear rightmost 2 bits, then set to 01
		}
		else{
			tmpB = (tmpB & 0xFC) | 0x02; // Sets tmpB to bbbbbb01
										// Clear rightmost 2 bits, then set to 10
		}
		// 3) Write output
		PORTB = tmpB & tmpB; // Writes port B's 8 pins with values
							// on port A's 8 pins
    }
	return 0;
}

