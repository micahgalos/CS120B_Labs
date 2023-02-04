/*
 * mgalo001_lab4_part3.c
 *
 * Created: 4/11/2019 10:00:01 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 4 Exercise 3
 * Exercise Description: A household has a digital combination deadbolt lock system on the doorway. The system has buttons on a keypad. 
 *					     Button 'X' connects to PA0, 'Y' to PA1, and '#' to PA2. Pressing and releasing '#', then pressing 'Y', should 
 *						 unlock the door by setting PB0 to 1. Any other sequence fails to unlock. Pressing a button from inside the 
 *						 house (PA7) locks the door (PB0=0). For debugging purposes, give each state a number, and always write the 
 *						 current state to PORTC (consider using the enum state variable). Also, be sure to check that only one button 
 *						 is pressed at a time. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

// Library Headers
#include <avr/io.h>

// Custom variables for SM
enum States {Init, S1, S2, S3, S4} state;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;

void Tick(){
	switch(state) { // Transitions
		case Init:
		state = S1;
		break;
		
		case S1:
		if(tmpA == 0x04){
		state = S2;
		}
		else if(tmpA == 0x80){
		state = S4;
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
		
		case S4:
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
		tmpB = 0x01;
		break;
		
		case S4:
		tmpB = 0x00;
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
	
	// Start execution here
	while (1)
	{
		tmpA = PINA;
		PORTB = tmpB;
		Tick();
		PORTC = state;
	}
}

