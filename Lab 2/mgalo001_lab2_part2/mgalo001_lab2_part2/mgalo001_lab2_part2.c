/*
 * mgalo001_lab2_part2.c
 *
 * Created: 4/3/2019 2:52:27 PM
 * Author: Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail:
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 2
 * Exercise Description: Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space, 
 *			   of a four-space parking lot. Write a program that outputs in binary on port C the number of available spaces 
 *			   (Hint: declare a variable "unsigned char cntavail"; you can assign a number to a port as follows: PORTC = cntavail;).
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Header
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs, initialize PORTC to 0
	unsigned char tmpA0 = 0x00; // PA0
	unsigned char tmpA1 = 0x00; // PA1
	unsigned char tmpA2 = 0x00; // PA2
	unsigned char tmpA3 = 0x00; // PA3
	unsigned char cntavail = 0x04; // Initialize count available to 4
	
	while (1)
	{
		// 1) Read Inputs PA0 - PA1 - PA2 - PA3
		tmpA0 = PINA & 0x01;
		tmpA1 = PINA & 0x03;
		tmpA2 = PINA & 0x07;
		tmpA3 = PINA & 0x0F;
		
		// If PA0, PA1, PA2, and PA3 is filled, decrement cntavail
		if(tmpA0 == 0x01){
			--cntavail;
		}
		if(tmpA1 == 0x03){
			--cntavail;
		}
		if(tmpA2 == 0x07){
			--cntavail;
		}
		if(tmpA3 == 0x0F){
			--cntavail;
		}
		// Output PORTC the available spaces
		PORTC = cntavail & cntavail;
		cntavail = 4;
	}
	return 0;
}

