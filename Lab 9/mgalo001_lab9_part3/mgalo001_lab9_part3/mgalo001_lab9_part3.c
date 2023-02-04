/*
 * mgalo001_lab9_part3.c
 *
 * Created: 5/5/2019 6:42:02 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 9 Exercise 3
 * Exercise Description: Using the ATmega1284’s built in PWM functionality, design a system where a short, five-second melody, 
 *						 is played when a button is pressed. NOTE: The melody must be somewhat complex (scaling from C to B is 
 *						 NOT complex).
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "pwm.h"
#include "frequency_definitions.h"

#define buttonA0 (~PINA & 0x01)
#define buttonA1 (~PINA & 0x02)
#define buttonA2 (~PINA & 0x04)

enum States{init, off, playTune, playTune1, playTune2}state;
						  
unsigned char i = 0x00;

void Tick(){

	switch(state){ // State Transitions
		case init:
		state = off;
		break;

		case off:
		if(buttonA0){
			state = playTune;
		}
		else if(buttonA1){
			state = playTune1;
		}
		else if(buttonA2){
			state = playTune2;
		}
		else{
			state = off;
			i = 0;
		}
		break;
		
		case playTune:
		if(i < (sizeof(DMT_Melody)/sizeof(DMT_Melody[0]))){
			state = playTune;
		}
		else {
			state = off;
		}
		i++;
		break;

		case playTune1:
		if(i < (sizeof(SS_Melody)/sizeof(SS_Melody[0]))){
			state = playTune1;
		}
		else {
			state = off;
		}
		i++;
		break;
				
		case playTune2:
		if(i < (sizeof(SMB_Melody)/sizeof(SMB_Melody[0]))){
			state = playTune2;
		}
		else {
			state = off;
		}
		i++;
		break;
		
		default:
		break;
	}
	
	switch(state){ // State Actions
		case init:
		break;

		case off:
		set_PWM(0);
		break;

		case playTune:
		TimerFlag = 0;
		TimerSet(DMT_Duration[i]);
		set_PWM(DMT_Melody[i]);
		break;
		
		case playTune1:
		TimerFlag = 0;
		TimerSet(SS_Duration[i]);
		set_PWM(SS_Melody[i]);
		break;
		
		case playTune2:
		TimerFlag = 0;
		TimerSet(SMB_Duration[i]);
		set_PWM(SMB_Melody[i]);
		break;				
		
		default:
		break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s

	//TimerSet(250);
	TimerOn();
	PWM_on();
	state = off;

	// Start Execution Here
	while(1){
		Tick();
		while(!TimerFlag){}
		//TimerFlag = 0;
	}
}
