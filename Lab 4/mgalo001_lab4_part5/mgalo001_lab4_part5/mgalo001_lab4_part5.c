/*
 * mgalo001_lab4_part5.c
 *
 * Created: 4/11/2019 10:49:30 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 4 Exercise 5
 * Exercise Description: (Challenge) Extend the above door to require the 4-button sequence #-X-Y-X rather than the earlier 
 *									 2-button sequence. To avoid excessive states, store the correct button sequence in an array, 
 *									 and use a looping SM. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

// Library Header
#include <avr/io.h>

// Custom variables for SM
enum States {Init, S1, S2} state;

unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
unsigned char sequence[4] = {0x04, 0x01, 0x02, 0x01};
unsigned char i = 0x00;

void Button_Sequence(){
	if(tmpA == sequence[i]) {
		++i;
	}
	else if(i != 0 && tmpA == sequence[i - 1]) {
		// Does nothing since user has not let go of previous input
	}
	else{
	i = 0x00;
	}
}

void Tick(){
	switch(state) { // State Transitions
		case Init:		// Initial State
		state = S1;
		break;
		
		case S1:		// Next Transition
		if(i == 4){
			state = S2;
			i = 0;
		}
		else{
		Button_Sequence();
		}
		break;
		
		case S2:		// Next Transition
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
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 pins as Inputs, Initialize to 1's
	DDRB = 0xFF; PORTB = 0x00; // Configure Port B's 8 pins as Outputs, Initialize to 0's
	DDRC = 0xFF; PORTC = 0x00; // Configure Port C's 8 pins as Outputs, Initialize to 0's
	
	state = Init;	// Initial State
	
	// Start Execution Here
    while (1)
    {
	    tmpA = PINA;
	    PORTB = tmpB;
	    Tick();
	    PORTC = state;
    }
}	