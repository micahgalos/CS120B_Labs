/*
 * mgalo001_lab5_part1.c
 *
 * Created: 4/15/2019 12:56:23 PM
 * Author : Micah Galos
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 5 Exercise 1
 * Exercise Description: (From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 
 *						 (full). A series of LEDs connected to PB5..PB0 should light to graphically indicate the fuel level. If the 
 *						 fuel level is 1 or 2, PB5 lights. If the level is 3 or 4, PB5 and PB4 light. 5-6 lights PB5..PB3. 7-9 lights 
 *						 PB5..PB2. 10-12 lights PB5..PB1. 13-15 lights PB5..PB0. Also, PB6 connects to a "Low fuel" icon, which should 
 *						 light if the level is 4 or less. Use buttons on PA3..PA0 and mimic the fuel-level sensor with presses. 
 *						 
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

// Library Headers
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 Pins as Inputs, Initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure Port C's 8 Pins as Outputs, Initialize to 0s
	
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	
	// Start Execution Here
	while (1)
	{
		// Read Input
		tempA = ~PINA & 0x0F;
		
		// Set tempB to 0
		tempB = 0x00;
		
		if(tempA == 1 || tempA == 2) {
			tempB = 0x60;
		}
		if(tempA == 3 || tempA == 4) {
			tempB = 0x70;
		}
		if(tempA == 5 || tempA == 6) {
			tempB = 0x38;
		}
		if(tempA >= 7 && tempA <= 9) {
			tempB = 0x3C;
		}
		if(tempA >= 10 && tempA <= 12) {
			tempB = 0x3E;
		}
		if(tempA >= 13 && tempA <= 15) {
			tempB = 0x3F;
		}
		// Write Output on Port B
		PORTB = tempB;
	}
}

