/*
 * mgalo001_lab7_part1.c
 *
 * Created: 4/25/2019 10:24:02 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 7 Exercise 1
 * Exercise Description: Buttons are connected to PA0 and PA1. Output PORTC and PORTD drive the LCD display, initially displaying 0. 
 *						 Pressing PA0 increments the display (stopping at 9). Pressing PA1 decrements the display (stopping at 0). 
 *						 If both buttons are depressed (even if not initially simultaneously), the display resets to 0. If a button is 
 *						 held, then the display continues to increment (or decrement) at a rate of once per second. Use a synchronous 
 *						 state machine captured in C.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

#define button_A0 (~PINA & 0x01)
#define button_A1 (~PINA & 0x02)

enum States {Start, Init, wait1, Inc, wait2, Dec, wait3, Reset, wait4} state;

unsigned char cnt = 0x00;
unsigned char i = 0x00;
volatile unsigned char TimerFlag = 0;

// Internal Variables for Mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR Timer / Counter Controller Register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR Timer Interrupt Mask Register
	TIMSK1 = 0x02; // bit1: OCIE1A -- Enables Compare Match Interrupt

	// Initialize AVR Counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	// Enable Global Interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: Timer Off
}

void TimerISR() {
	TimerFlag = 1;
}

// In this approach, the C Programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) {	// Results in a more efficient Compare
		TimerISR();	// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void Tick()
{
	switch(state) // Start of State Transitions
	{
		case Start:
		state = Init;
		break;
		
		case Init:
		state = wait1;
		break;
		
		case wait1:
		if(button_A0){
			state = Inc;
		}
		else if(button_A1){
			state = Dec;
		}
		else if(button_A0 && button_A1){
			state = Reset;
		}
		else{
			state = wait1;
		}
		break;

		case Inc:
		if(!button_A0 && !button_A1){
			if(cnt < 9)
			++cnt;
			state = wait1;
		}
		else if (button_A0 && !button_A1){
			state = Inc;
		}
		else if(button_A0 && button_A1){
			state = Reset;
		}
		break;

		case Dec:
		if(!button_A0 && !button_A1){
			if(cnt > 0)
			--cnt;
			state = wait1;
		}
		else if (!button_A0 && button_A1){
			state = Dec;
		}
		else if(button_A0 && button_A1){
			state = Reset;
		}
		break;

		case Reset:
		state = Init;
		break;

		default:
		state = Start;
		break;
	}	// End of State Transitions
	
	switch(state)	// Start of State Actions
	{
		case Start:
		break;

		case Init:
		i = 0x00;
		cnt = 0x00;
		LCD_init();
		LCD_WriteData(cnt + '0');
		break;
		
		case Inc:
		if(i < 9){
			i++;
		}
		if(i == 9 && cnt < 9){
			++cnt;
			i = 0;
		}
		LCD_init();
		LCD_WriteData(cnt + '0');
		break;

		case Dec:
		if(i < 9){
			i++;
		}
		if(i == 9 && cnt > 0){
			--cnt;
			i = 0;
		}
		LCD_init();
		LCD_WriteData(cnt + '0');
		break;

		case Reset:
		cnt = 0x00;
		break;

		default:
		break;
	}	// End of State Actions
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 pins as Inputs, Initialize to 1's
	DDRC = 0xFF; PORTC = 0x00; // LCD Data Lines
	DDRD = 0xFF; PORTD = 0x00; // LCD Control Lines
	
	state = Start;
	TimerSet(100);
	TimerOn();
	LCD_init();
	
	while (1)
	{
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}
