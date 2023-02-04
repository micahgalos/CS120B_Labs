/*
 * timer.h
 *
 * Created: 5/5/2019 6:46:57 PM
 *  Author: Mike Galos
 */ 


#ifndef TIMER_H_
#define TIMER_H_

volatile unsigned char TimerFlag = 0;

// Internal Variables for Mapping AVR's ISR to our Cleaner TimerISR Model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR Timer / Counter Controller Register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 Register will Count at 125,000 ticks/s

	// AVR Output Compare Register OCR1A.
	OCR1A = 125;	// Timer Interrupt will be Generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR Timer Interrupt Mask Register
	TIMSK1 = 0x02; // bit1: OCIE1A -- Enables Compare Match Interrupt

	// Initialize AVR Counter
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
	unsigned char i;
	for (i = 0;i < tasksSize;++i) {
		if (tasks[i].elapsedTime >= tasks[i].period) {
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += tasksPeriod;
	}
}
// In our approach, the C Programmer utilizes TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn Settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // Results in a more efficient Compare
		TimerISR(); // Call the ISR that the user chooses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

#endif /* TIMER_H_ */