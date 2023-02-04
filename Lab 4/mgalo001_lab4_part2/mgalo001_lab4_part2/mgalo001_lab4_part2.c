/*
 * mgalo001_lab4_part2.c
 *
 * Created: 4/11/2019 9:37:18 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 4 Exercise 2
 * Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0
 *						 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). 
 *						 If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.						 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Headers
#include <avr/io.h>

// Bit-Access Functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

// Custom variables for SM
enum States {Start, Init, Inc, Dec, Reset} state;
	
	unsigned char tmpA = 0x00;
	unsigned char tmpC = 0x00;

void Tick(){
	switch(state){ // Transitions
		case Start:
		state = Init; // --> Initial State
		break;
		
		case Init:
		if(tmpA == 0x01){
		state = Inc; // --> Increment State
		}
		else if(tmpA == 0x02){
		state = Dec; // --> Decrement State
		}
		else if(tmpA == 0x03){
		state = Reset; // --> Reset State
		}
		else{
		state = Init; // --> Initial State
		}
		break;
		
		case Inc:
		state = Init; // --> Initial State
		break;
		
		case Dec:
		state = Init; // --> Initial State
		break;
		
		case Reset:
		state = Init; // --> Initial State
		break;
		
		default:
		state = Start; // --> Start State
		break;
	}
	
	switch(state){ // State Actions
		case Start: 
		break;
		
		case Init:
		break;
		
		case Inc:
		if(tmpC < 9){
		++tmpC;
		}
		break;
		
		case Dec:
		if(tmpC > 0){
		--tmpC;
		}
		break;
		
		case Reset:
		tmpC = 0x00;
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 pins as Inputs, Initialize to 1's.
	DDRC = 0xFF; PORTC = 0x00; // Configure Port C's 8 pins as Outputs, Initialize to 0's.
	
	tmpC = 0x07; // Initialize to 7 for output
	state = Start;
	
	while (1)
	{
		tmpA = PINA; // Reads inputs of PINA
		PORTC = tmpC; // Outputs the inc or dec value of tempC
		Tick();
	}
}

