/*
 * mgalo001_lab4_part4.c
 *
 * Created: 4/11/2019 10:28:59 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 4 Exercise 4
 * Exercise Description: (Challenge) Extend the above door so that it can also be locked by entering the earlier code.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

// Library Headers
#include <avr/io.h>

// Custom variables for SM
enum States {Init, S1, S2, S3} state;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

void Tick(){
	switch(state) { // State Transitions
		case Init:
		state = S1;
		break;
		
		case S1:
		if(tmpA == 0x04){
		state = S2;
		}
		else{
		state = S1;
		}
		break;
		
		case S2:
		if(tmpA == 0x02){
		state = S3;
		}
		else{
		state = S1;
		}
		break;
		
		case S3:
		state = S1;
		break;
		
		default:
		state = Init;
		break;
	}
	
	switch(state) { // State Actions
		case Init:
		break;
		
		case S1:
		break;
		
		case S2:
		break;
		
		case S3:
		if(tmpB == 0x01){
		tmpB = 0x00;
		}
		else{
		tmpB  = 0x01;
		}
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 pins as Inputs, Initialize to 1's.
	DDRB = 0xFF; PORTB = 0x00; // Configure Port B's 8 pins as Outputs, Initialize to 0's.
	DDRC = 0xFF; PORTC = 0x00; // Configure Port C's 8 pins as Outputs, Initialize to 0's.
	
	state = Init;
	
	// Start Execution Here
	while (1)
	{
		tmpA = PINA;
		PORTB = tmpB;
		Tick();
		PORTC = state;
	}
}

