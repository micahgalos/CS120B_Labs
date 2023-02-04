/*
 * mgalo001_lab9_part1.c
 *
 * Created: 5/5/2019 6:28:47 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 9 Exercise 1
 * Exercise Description: Using the ATmega1284’s PWM functionality, design a system that uses three buttons to select one of three 
 *						 tones to be generated on the speaker. When a button is pressed, the tone mapped to it is generated on the 
 *						 speaker.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"

#define button (~PINA & 0x07)

enum States{wait, cFour, dFour, eFour}state;

void Tick(){ // Start of Tick
	switch(state){ // State Transitions
		case wait:
		if(button == 0x01){
			state = cFour;
		}
		else if(button == 0x02){
			state = dFour;
		}
		else if(button == 0x04){
			state = eFour;
		}
		else{
			state = wait;
		}
		break;

		case cFour:
		if(button == 0x01){
			state = cFour;
		}
		else{
			state = wait;
		}
		break;

		case dFour:
		if(button == 0x02){
			state = dFour;
		}
		else{
			state = wait;
		}
		break;

		case eFour:
		if(button == 0x04){
			state = eFour;
		}
		else{
			state = wait;
		}
		break;

		default:
		break;
	}
	
	switch(state){ // State Actions
		case wait:
		set_PWM(0);
		break;

		case cFour:
		set_PWM(261.63);
		break;

		case dFour:
		set_PWM(293.66);
		break;

		case eFour:
		set_PWM(329.63);
		break;

		default:
		break;
	}
} // End of Tick


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	PWM_on();
	
	state = wait;
	
	// Start Execution Here
	while(1){
		Tick();
	}
}

