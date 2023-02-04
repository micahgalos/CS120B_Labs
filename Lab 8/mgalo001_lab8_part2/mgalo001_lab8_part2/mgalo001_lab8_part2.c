/*
 * mgalo001_lab8_part2.c
 *
 * Created: 4/29/2019 1:27:28 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 8 Exercise 2
 * Exercise Description: Revise Part 1 by replacing the potentiometer with a photoresistor and 330
 *						 ohm resistor. Take note of the highest ADC value displayed when the photoresistor is exposed to light, 
 *						 and the lowest ADC value displayed when the photoresistor is deprived of all light. These values will 
 *						 be used for the remaining lab exercises.
 *
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
	
	unsigned short my_short = 0x00;
	
	ADC_init();
	
	// Start Execution Here
	while (1)
	{
		my_short = ~ADC;
		PORTB = (char) my_short;
		PORTD = (char) (my_short >> 4);
	}
}

