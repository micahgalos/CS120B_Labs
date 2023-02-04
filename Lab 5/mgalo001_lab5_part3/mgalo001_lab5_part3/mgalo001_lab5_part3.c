/*
 * mgalo001_lab5_part3.c
 *
 * Created: 4/16/2019 4:39:12 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 5 Exercise 3
 * Exercise Description: Create your own festive lights display with 6 LEDs connected to port PB5..PB0, lighting in some attractive 
 *						 sequence. Pressing the button on PA0 changes the lights to the next configuration in the sequence.
 *						 Use a state machine (not synchronous) captured in C
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
enum States {Start, S1, wait} state;

unsigned char pattern1[] = {0x21, 0x12, 0x0C, 0x12};
unsigned char button = 0x00;
unsigned char i = 0x00;

void Run_Pattern(unsigned char pattern[], unsigned char length)
{
	if(i == length)
	i = 0x00;
	PORTB = pattern[i];
}

void Tick()
{
	switch(state)
	{
		case Start:
		state = S1;
		break;
		
		case S1:
		if(button)
		{
			state = wait;
		}
		break;
		
		case wait:
		if(!button)
		{
			++i;
			state = S1;
		}
		break;
		
		default:
		state = Start;
		break;
		
	}
	
	switch(state)
	{
		case Start:
		break;
		
		case S1:
		Run_Pattern(pattern1, 4);
		break;
		
		case wait:
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 Pins as Inputs, Initialize to 1s
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 Pins as Outputs, Initialize to 0s
	
	state = Start;
	
	// Start execution here
	while (1)
	{
		button = ~PINA & 0x01;
		Tick();
	}
}

