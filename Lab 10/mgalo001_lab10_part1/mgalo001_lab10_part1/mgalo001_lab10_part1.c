/*
 * mgalo001_lab10_part1.c
 *
 * Created: 5/5/2019 7:12:49 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 10 Exercise 1
 * Exercise Description: Connect LEDs to PB0, PB1, PB2, and PB3. In one state machine (Three LEDs), output to a shared variable 
 *						 (threeLEDs) the following behavior: set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 
 *						 1 second each. In a second state machine (Blinking LED), output to a shared variable (bilnkingLED) the 
 *						 following behavior: set bit 3 to 1 for 1 second, then 0 for 1 second. In a third state machine (Combine LEDs), 
 *						 combine both shared variables and output to the PORTB. Note: only one SM is writing to outputs. 
 *						 Do this for the rest of the quarter.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

enum BL_States { BL_SMStart, BL_LedOff, BL_LedOn } BL_State;
enum TL_States { TL_SMStart, TL_T0, TL_T1, TL_T2 } TL_State;
enum CL_States { CL_SMStart, Combine} CL_State;

unsigned char threeLed = 0x00;
unsigned char blinkingLed = 0x00;

void TickFct_BlinkLed() {
	switch(BL_State){ // BL State Transitions
		case BL_SMStart:
		BL_State = BL_LedOff;
		break;
		
		case BL_LedOff:
		BL_State = BL_LedOn;
		break;
		
		case BL_LedOn:
		BL_State = BL_LedOff;
		break;
		
		default:
		break;
	}
	
	switch(BL_State){ // BL State Actions
		case BL_SMStart:
		blinkingLed = 0x00;
		break;
		
		case BL_LedOff:
		blinkingLed = 0x00;
		break;
		
		case BL_LedOn:
		blinkingLed = 0x01;
		break;
		
		default:
		break;
	}
}

void TickFct_ThreeLeds() {
	switch(TL_State){ // TL State Transitions
		case TL_SMStart:
		TL_State = TL_T0;
		break;
		
		case TL_T0:
		TL_State = TL_T1;
		break;
		
		case TL_T1:
		TL_State = TL_T2;
		break;
		
		case TL_T2:
		TL_State = TL_T0;
		break;
		
		default:
		break;
	}
	
	switch(TL_State){ // TL State Actions
		case TL_SMStart:
		threeLed = 0x00;
		break;
		case TL_T0:
		threeLed = 0x01;
		break;
		case TL_T1:
		threeLed = 0x02;
		break;
		case TL_T2:
		threeLed = 0x04;
		break;
		default:
		break;
	}
}

void TickFct_CombineLeds() {
	switch(CL_State){ // CL State Transitions
		case CL_SMStart:
		CL_State = Combine;
		break;
		
		case Combine:
		CL_State = Combine;
		break;
		
		default:
		break;
	}
	switch(CL_State){ // CL State Actions
		case CL_SMStart:
		PORTB = 0x00;
		break;
		
		case Combine:
		PORTB = (blinkingLed << 3) | threeLed;
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	TimerSet(250);
	TimerOn();
	BL_State = BL_SMStart;
	TL_State = TL_SMStart;
	CL_State = CL_SMStart;
	
	// Start Execution Here
	while (1) {
		TickFct_BlinkLed();    // Tick the BlinkLed synchSM
		TickFct_ThreeLeds();   // Tick the threeLed synchSM
		TickFct_CombineLeds();
		while (!TimerFlag){}   // Wait for timer period
		TimerFlag = 0;         // Lower flag raised by timer
	}
}
