/*
 * mgalo001_lab2_part3.c
 *
 * Created: 4/3/2019 3:28:49 PM
 * Author: Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail:
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 3
 * Exercise Description: Extend the previous program to still write the available spaces number, 
 *			   but only to PC3..PC0, and to set PC7 to 1 if the lot is full.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Header
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00;	// Configure port C's 8 pins as outputs, Initialize PORTC to 0
	unsigned char tmpA0 = 0x00; // PA0
	unsigned char tmpA1 = 0x00; // PA1
	unsigned char tmpA2 = 0x00; // PA2
	unsigned char tmpA3 = 0x00; // PA3
	unsigned char tmpC7 = 0x00; // PC7
	unsigned char cntavail = 0x04; // Max size of available spaces to 4
	
	while (1)
	{
		// 1) Read Inputs PA0 - PA1 - PA2 - PA3
		tmpA0 = PINA & 0x01;
		tmpA1 = PINA & 0x03;
		tmpA2 = PINA & 0x07;
		tmpA3 = PINA & 0x0F;
		
		// If PA0, PA1, PA2, PA3 are filled, decrement cntavail
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
		
		// If no spaces are available, cntavail is set the rightmost bit of PC7
		if(cntavail == 0x00){
			cntavail = tmpC7 | 0x80;
		}
		
		// Output PORTC the available spaces
		PORTC = cntavail & cntavail;
		cntavail = 0x04;
	}
	return 0;
}

