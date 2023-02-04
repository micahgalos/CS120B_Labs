/*
 * mgalo001_lab2_part1.c
 *
 * Created: 4/3/2019 9:20:13 AM
 * Author: Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail:
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 1
 * Exercise Description: Garage open at night-- A garage door sensor connects to PA0 (1 means door open),
 *			   and a light sensor connects to PA1 (1 means light is sensed). Write a program that
 *			   that illuminates the LED connected to PB0(1 means illuminate) if the garage is opened
 *			   at night.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

// Library Header
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as outputs
	DDRB = 0xFF; PORTB = 0x00;	// Configure port B's 8 pins as outputs, initialize PORTB to 0
	unsigned char tmpB = 0x00;  // PB0
	unsigned char tmpA0 = 0x00; // PA0 - Garage Sensor Input
	unsigned char tmpA1 = 0x00; // PA1 - Light Sensor Input
	
	while (1)
	{
		// 1) Read Inputs PA0 and PA1
		tmpA0 = PINA & 0x01;
 		tmpA1 = PINA & 0x03;
		
		// 2) Perform computation
		// If PA0 is 1 and PA1 is 0, PB0 = 1, else = 0
		if( (tmpA0 == 0x01) && (tmpA1 == 0x00) ){ // True if PA1 is 0 and PA0 is 1
			tmpB = (tmpB & 0xFE) | 0x01; // Sets tmpB to bbbbbb01
										 // Clear rightmost 2 bits, then set to 1
		}
		
		else{ // False
			tmpB = (tmpB & 0xFE) | 0x00; // Sets tmpB to bbbbbb10
										 // Clear rightmost 2 bits, then set to 0
		}
		// 3) Write output
		PORTB = tmpB & tmpB; // Writes port B's 8 pins with values
					  // on port A's 8 pins
	}
	return 0;
}

