/*
 * mgalo001_lab10_part4.c
 *
 * Created: 5/5/2019 7:36:37 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 10 Exercise 4
 * Exercise Description: Extend the previous exercise to allow a user to adjust the sound frequency up or down using buttons connected 
 *						 to PA0 (up) and PA1 (down). Using our 1 ms timer abstraction, the fastest you'll be able to pulse is 1 ms on 
 *						 and 1 ms off, meaning 500 Hz. You'll probably want to introduce another synchSM that polls the buttons and sets
 *						 a global variable storing the current frequency that in turn is read by the frequency generator task.
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
enum P_States { P_SMStart, P_Wait, P_Up, P_WaitUp, P_Down, P_WaitDown} P_State;

#define button_A0 (~PINA & 0x01)
#define button_A1 (~PINA & 0x02)
#define switch_A2 (~PINA & 0x04)

unsigned char threeLed = 0x00;
unsigned char blinkingLed = 0x00;
const double notes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25}; // C4, D4, E4, F4, G4, A4, B4, C5
unsigned char i = 0x00;

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
		set_PWM(notes[i]);
		break;

		default:
		break;
	}
}

void TickFct_Pitch() {
	switch(P_State) {
		case S_SMStart:
		P_State = P_Wait;
		break;
		case P_Wait:
		if(button_A0){
		P_State = P_WaitUp;
		}
		else if(button_A1){
		P_State = P_WaitDown;
		}
		else{
		P_State = P_Wait;
		}
		break;
		
		case P_Up:
		P_State = P_Wait;
		break;
		
		case P_WaitUp:
		if(button_A0){
		P_State = P_WaitUp;
		}
		else{
		P_State = P_Up;
		}
		break;
		
		case P_Down:
		P_State = P_Wait;
		break;
		
		case P_WaitDown:
		if(button_A1){
		P_State = P_WaitDown;
		}
		else{
		P_State = P_Down;
		}
		break;
		
		default:
		break;
	}
	
	switch(P_State) {
		case S_SMStart:
		i = 0;
		break;
		
		case P_Wait:
		break;
		
		case P_Up:
		if(i < 7){
		i++;
		}
		break;
		
		case P_WaitUp:
		break;
		
		case P_Down:
		if(i > 0){
		i--;
		}
		break;
		
		case P_WaitDown:
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
		if (BL_elapsedTime >= 1000) { // 1000 ms period
			TickFct_BlinkLed(); // Execute one tick of the BlinkLed synchSM
			BL_elapsedTime = 0;
		}
		if (TL_elapsedTime >= 300) { // 300 ms period
			TickFct_ThreeLeds(); // Execute one tick of the ThreeLeds synchSM
			TL_elapsedTime = 0;
		}
		if (TL_elapsedTime >= 2) { // 2 ms period
			TickFct_Speaker(); // Execute one tick of the Speaker synchSM
			S_elapsedTime = 0;
		}
		TickFct_CombineLeds();
		TickFct_Pitch();
		while (!TimerFlag){}   // Wait for timer period
		TimerFlag = 0;         // Lower flag raised by timer
		BL_elapsedTime += timerPeriod;
		TL_elapsedTime += timerPeriod;
		S_elapsedTime += timerPeriod;
	}
}

