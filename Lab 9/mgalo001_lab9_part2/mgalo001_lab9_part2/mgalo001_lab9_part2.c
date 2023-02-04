/*
 * mgalo001_lab9_part2.c
 *
 * Created: 5/5/2019 6:38:23 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 9 Exercise 2
 * Exercise Description: Using the ATmega1284’s PWM functionality, design a system where the notes: C4, D, E, F, G, A, B, and C5,  
 *						 from the table at the top of the lab, can be generated on the speaker by scaling up or down the eight note 
 *						 scale. Three buttons are used to control the system. One button toggles sound on/off. The other two buttons 
 *						 scale up, or down, the eight note scale. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"

enum States{off, on, turnOn, turnOff, playNote, up, waitUp, down, waitDown}state;

unsigned char button = 0x00;

const double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25}; // C4, D4, E4, F4, G4, A4, B4, C5

unsigned char i = 0x00;

void Tick(){

	switch(state){ // State Transitions
		case off:
		if(button == 0x01){
			state = turnOn;
		}
		else{
			state = off;
		}
		break;

		case turnOff:
		if(button == 0x01){
			state = turnOff;
		}
		else{
			state = off;
		}
		break;

		case turnOn:
		if(button == 0x01){
			state = turnOn;
		}
		else{
			state = on;
		}
		break;
		case on:
		state = playNote;
		break;

		case playNote:
		if(button == 0x01){
			state = turnOff;
		}
		else if(button == 0x02){
			state = waitUp;
		}
		else if(button == 0x04){
			state = waitDown;
		}
		else{
			state = playNote;
		}
		break;

		case up:
		state = playNote;
		break;

		case waitUp:
		if(button == 0x02){
			state = waitUp;
		}
		else{
			state = up;
		}
		break;

		case down:
		state = playNote;
		break;

		case waitDown:
		if(button == 0x04){
			state = waitDown;
		}
		else{
			state = down;
		}
		break;

		default:
		break;
	}
	
	switch(state){ // State Actions
		case off:
		PWM_off();
		break;

		case turnOn:
		break;
		
		case turnOff:
		break;
		
		case on:
		PWM_on();
		i = 0x00;
		break;

		case playNote:
		set_PWM(notes[i]);
		break;

		case up:
		if(i < 7){
			++i;
		}
		break;

		case waitUp:
		break;
		
		case down:
		if(i > 0){
			--i;
		}
		break;

		case waitDown:
		break;

		default:
		break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	PWM_on();
	
	state = off;
	
	// Start Execution Here
	while(1){
		button = ~PINA & 0x07;
		Tick();
	}
}

