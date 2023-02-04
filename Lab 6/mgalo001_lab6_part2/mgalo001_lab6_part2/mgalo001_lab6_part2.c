/*
 * mgalo001_lab6_part2.c
 *
 * Created: 4/22/2019 1:06:16 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 6 Exercise 2
 * Exercise Description: (Challenge) Create a simple light game that requires pressing a button on PA0 while the middle of three 
 *									 LEDs on PB0, PB1, and PB2 is lit. The LEDs light for 300 ms each in sequence. When the button 
 *									 is pressed, the currently lit LED stays lit. Pressing the button again restarts the game. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

enum States {Start, Init, NextLED, Wait, Restart} state;

unsigned char pattern[] = {0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04};
#define button (~PINA & 0x01)
unsigned char i = 0x00;
volatile unsigned char TimerFlag = 0;

// Internal Variables for Mapping AVR's ISR to our Cleaner TimerISR Model.
unsigned long _avr_timer_M = 1; // Start Count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current Internal Count of 1ms Ticks

void TimerOn() {
	// AVR (Timer / Counter) Controller Register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (Clear Timer on Compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 Register will Count at 125,000 ticks/s

	// AVR Output Compare Register OCR1A.
	OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02;	// bit1: OCIE1A -- Enables Compare Match Interrupt

	// Initialize AVR Counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	// Enable Global Interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00;	// bit3bit1bit0=000: Timer Off
}

void TimerISR() {
	TimerFlag = 1;
}
// In our approach, the C Programmer utilizes TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn Settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) {	// Results in a more efficient Compare
		TimerISR();		// Call the ISR the user chooses
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
	switch(state) // State Transitions
	{
		case Start:
		state = Init;
		break;

		case Init:
		state = NextLED;
		break;

		case NextLED:
		if(button){
		state = Wait;
		}
		else{
		state = NextLED;
		}
		break;

		case Wait:
		if(button){
		state = Wait;
		}
		else{
		state = Restart;
		}
		break;

		case Restart:
		if(button){
		state = Init;
		}
		else{
		state = Restart;
		}
		break;

		default:
		state = Start;
		break;
	}

	switch(state) // State Actions
	{
		case Start:
		break;

		case Init:
		i = 0;
		break;

		case NextLED:
		i++;
		if(i == 9){
		i = 0;
		}
		PORTB = pattern[i];
		break;

		case Wait:
		break;

		case Restart:
		break;

		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure Port A's 8 pins as Inputs, Initialize to 1's
	DDRB = 0xFF; PORTB = 0x00; // Configure Port B's 8 pins as Outputs, Initialize to 0's
	
	state = Start;
	TimerSet(100);
	TimerOn();
	
	// Start Execution Here
	while (1)
	{
		Tick();
		while(!TimerFlag){}
		TimerFlag = 0;
	}
}

