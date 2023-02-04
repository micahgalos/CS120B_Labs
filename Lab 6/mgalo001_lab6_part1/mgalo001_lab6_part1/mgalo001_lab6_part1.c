/*
 * mgalo001_lab6_part1.c
 *
 * Created: 4/22/2019 11:10:32 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 6 Exercise 1
 * Exercise Description: Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. 
 *						 Implement that synchSM in C using the method defined in class. In addition to demoing your program, 
 *						 you will need to show that your code adheres entirely to the method with no variations.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

enum States {Start, Init, NextLed} state;

unsigned char pattern1[] = {0x04, 0x02, 0x01};
unsigned char out = 0x00;
volatile unsigned char TimerFlag = 0;

// Internal Variables for Mapping AVR's ISR to our cleaner TimerISR Model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current Internal Count of 1ms Ticks

void TimerOn() {
	// AVR (Timer / Counter) Controller Register TCCR1
	TCCR1B = 0x0B;	// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// So, 8 MHz Clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 Register will Count at 125,000 ticks/s

	// AVR Output Compare Register OCR1A.
	OCR1A = 125;	// Timer Interrupt will be Generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 Register equals 125,
	// 1 ms has passed. So then we Compare to 125.
	// AVR Timer Interrupt Mask Register
	TIMSK1 = 0x02;	// bit1: OCIE1A -- Enables Compare Match Interrupt

	//Initialize AVR Counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable Global Interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: Timer Off
}

void TimerISR() {
	TimerFlag = 1;
}

// In this approach, the C Program uses TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--;	// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient Compare
		TimerISR();		// Call on the ISR the User chooses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to Tick every M(ms)
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void Tick()
{
	switch(state)
	{
		case Start:
		state = Init;
		break;

		case Init:
		state = NextLed;
		break;

		case NextLed:
		state = NextLed;
		break;

		default:
		state = Start;
		break;
	}

	switch(state)
	{
		case Start:
		break;
		
		case Init:
		PORTB = out = 0x01;
		break;

		case NextLed:
		if(out < 0x04){
		out = out << 1;
		}
		else{
		out = 0x01;
		}
		PORTB = out;
		break;

		default:
		break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // Configure Port B's 8 pins as Outputs, Initialize to 0's
	
	state = Start;
	TimerSet(1000);
	TimerOn();
	
	while (1)
	{
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}
