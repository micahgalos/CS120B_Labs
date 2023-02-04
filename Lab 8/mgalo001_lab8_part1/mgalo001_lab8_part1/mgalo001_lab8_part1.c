/*
 * mgalo001_lab8_part1.c
 *
 * Created: 4/29/2019 12:04:53 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 8 Exercise 1
 * Exercise Description: Make sure your breadboard is wired according to the prelab. The potentiometer is used to adjust the 
 *						 voltage supplied to the microcontroller for ADC . Design a system that reads the 10-bit ADC result from 
 *						 the ADC register, and displays the result on a bank of 10 LEDs.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned short myShort = 0x00;
	
	ADC_init();
	
	// Start Execution Here
	while (1)
	{
		myShort = ADC;
		PORTB = (char) myShort;
		PORTD = (char) (myShort >> 4);
	}
}


