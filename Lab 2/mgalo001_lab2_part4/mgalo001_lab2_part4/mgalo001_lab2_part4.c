/*
 * mgalo001_lab2_part4.c
 *
 * Created: 4/4/2019 10:43:15 AM
 * Author: Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail:
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 4
 * Exercise Description: An amusement park kid ride cart has three seats, with 8-bit weight sensors connected to ports A, B, and C 
 *			   (measuring from 0-255 kilograms). Set PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. 
 *			   Also, the cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg. 
 *			   Can you also devise a way to inform the ride operator of the approximate weight using the remaining bits on D? 
 *			   (Interesting note: Disneyland recently redid their "It's a Small World" ride because the average passenger weight has 
 *			   increased over the years, causing more boats to get stuck on the bottom).
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Header
#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;	// Configure port A's 8 pins as outputs
	DDRB = 0x00; PORTB = 0xFF;	// Configure port B's 8 pins as outputs
	DDRC = 0x00; PORTC = 0xFF;	// Configure port C's 8 pins as outputs
	DDRD = 0xFF; PORTD = 0x00;	// Configure port D's 8 pins as outputs and initialize PORTD to 0
	
	// Initial all PIN[Letter]s to 0
	unsigned char tmpA = 0x00; // PA0
	unsigned char tmpB = 0x00; // PB0
	unsigned char tmpC = 0x00; // PC0
	unsigned char tmpD = 0x00; // PD0
	
	// Calculation variables for total weight and the weight balance
	unsigned long totWeight = 0x00;
	unsigned long wBalance = 0x00;
	
	while (1)
	{
		// 1) Read Inputs PA0 - PB0 - PC0
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC; // NOTE: debugging using the middle four bits will cause those inputs to disappear, USE FIRST OR LAST TWO 
		
		// total weight is the sum of PA0, PB0, and PC0
		totWeight = tmpA + tmpB + tmpC;
		
		// weight balance is the difference of PA0 and PC0
		wBalance = tmpA - tmpC;
		
		// if the total weight exceeds 140 kg
		if(totWeight > 140){ // in kilograms
			tmpD = tmpD | 0x01; // set PD0's output bit 0 to 1
		}
		
		// if the weight balance exceeds 80 kg
		if(wBalance > 80){ // in kilograms
			tmpD = tmpD | 0x02; // set PD0's output bit 1 to 1
		}
		PORTD = tmpD & tmpD;	// Output the bit result of PD0
		tmpD = 0x00;	// Reinitialize PD0 to 0
	}
	return 0;
}

