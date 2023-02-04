/*
 * mgalo001_lab10_part3.c
 *
 * Created: 5/5/2019 7:28:21 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 10 Exercise 3
 * Exercise Description: To the previous exercise's implementation, connect your speaker's red wire to PB4 and black wire to ground. 
 *						 Add a third task that toggles PB4 on for 2 ms and off for 2 ms as long as a switch on PA2 is in the on position. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "pwm.h"

enum BL_States { BL_SMStart, BL_LedOff, BL_LedOn } BL_State;
enum TL_States { TL_SMStart, TL_T0, TL_T1, TL_T2 } TL_State;
enum CL_States { CL_SMStart, Combine} CL_State;
enum S_States { S_SMStart, S_Off, S_On} S_State;

#define switch_A2 (~PINA & 0x04)

unsigned char threeLed = 0x00;
unsigned char blinkingLed = 0x00;

void TickFct_BlinkLed() {
	switch(BL_State){
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
	
	switch(BL_State){
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
	switch(TL_State){
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
	switch(TL_State){
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
	switch(CL_State){
		case CL_SMStart:
		CL_State = Combine;
		break;
		
		case Combine:
		CL_State = Combine;
		break;
		
		default:
		break;
	}
	
	switch(CL_State){
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

void TickFct_Speaker() {
	switch(S_State) {
		case S_SMStart:
		S_State = S_Off;
		break;
		
		case S_Off:
		if(switch_A2){
		S_State = S_On;
		}
		else{
		S_State = S_Off;
		}
		break;
		
		case S_On:
		if(!switch_A2){
		S_State = S_Off;
		}
		else{
		S_State = S_On;
		}
		break;
		
		default:
		break;
	}
	switch(S_State) {
		case S_SMStart:
		break;
		
		case S_Off:
		set_PWM(0);
		break;
		
		case S_On:
		set_PWM(523.25);
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs, initialize to 0s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	
	unsigned long BL_elapsedTime = 1000;
	unsigned long TL_elapsedTime = 300;
	unsigned long S_elapsedTime = 2;
	const unsigned long timerPeriod = 1;
	
	TimerSet(timerPeriod);
	TimerOn();
	PWM_on();
	BL_State = BL_SMStart;
	TL_State = TL_SMStart;
	CL_State = CL_SMStart;
	S_State = S_SMStart;
	
	// Start Execution Here
	while (1) {
		if (TL_elapsedTime >= 2) { // 2 ms period
			TickFct_Speaker(); // Execute one tick of the Speaker synchSM
			S_elapsedTime = 0;
		}
		if (TL_elapsedTime >= 300) { // 300 ms period
			TickFct_ThreeLeds(); // Execute one tick of the ThreeLeds synchSM
			TL_elapsedTime = 0;
		}				
		if (BL_elapsedTime >= 1000) { // 1000 ms period
			TickFct_BlinkLed(); // Execute one tick of the BlinkLed synchSM
			BL_elapsedTime = 0;
		}
		TickFct_CombineLeds();
		while (!TimerFlag){}   // Wait for timer period
		TimerFlag = 0;         // Lower flag raised by timer
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
		S_elapsedTime += timerPeriod;
	}
}

