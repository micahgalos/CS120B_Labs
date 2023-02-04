/*
 * mgalo001_lab8_part4.c
 *
 * Created: 4/29/2019 11:17:22 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 8 Exercise 4
 * Exercise Description: Design a system, using a bank of eight LEDs, where the number of LEDs illuminated is a representation of 
 *						 how much light is detected. For example, when more light is detected, more LEDs are illuminated.
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
	DDRB = 0xFF; PORTB = 0x00; // B to output
	DDRD = 0xFF; PORTD = 0x00; // D to output
	
	unsigned short max = 50;
	unsigned char tempB = 0x00;
	unsigned short x = 0x00;
	
	ADC_init();
	
	// Start Execution Here
	while(1){
		x = ADC;
		
		if( x <= 0 ){
			tempB = 0x00;
		}
		else if (x <= max * .125){ //multiply max by 1/8
			tempB = 0x01;
		}
		else if( x <= max * .25){ //multiply max by 1/4
			tempB = 0x02;
		}
		else if( x <= max * .375){ //multiply max by 3/8
			tempB = 0x04;
		}
		else if( x <= max * .50){ //multiply max by 1/2
			tempB = 0x08;
		}
		else if( x <= max * .625){ //multiply max by 5/8
			tempB = 0x10;
		}
		else if( x <= max * .75){ //multiply max by 3/4
			tempB = 0x20;
		}
		else if( x <= max * .875){ //multiply max by 7/8
			tempB = 0x40;
		}
		else if( x <= max){
			tempB = 0x80;
		}

		PORTB = tempB;
	}
}

