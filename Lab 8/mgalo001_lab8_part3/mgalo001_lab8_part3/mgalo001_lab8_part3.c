/*
 * mgalo001_lab8_part3.c
 *
 * Created: 4/29/2019 1:56:37 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 8 Exercise 3
 * Exercise Description: Design a system where an LED is illuminated if enough light is detected from the photo resistor.
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
	
	unsigned short max = 100;
	unsigned char tempB = 0x00;
	unsigned short x = 0x00;
	
	ADC_init();
	
	while(1){
		x = ADC;
		if(x >= max){
			max = x;
		}
		if (x < max/2){
			tempB = 0x00;
		}
		else{
			tempB = 0x01;
		}
		PORTB = tempB;
	}
}

