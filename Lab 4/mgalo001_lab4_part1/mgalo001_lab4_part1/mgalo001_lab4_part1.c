/*
 * mgalo001_lab4_part1.c
 *
 * Created: 4/11/2019 9:16:36 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 4 Exercise 1
 * Exercise Description: PB0 and PB1 each connect to an LED, and PB0's LED is initially on. Pressing a button
						 connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after
						 button release. Pressing the button again turns off PB1's LED and turns on PB0's LED. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 

// Library Header
#include <avr/io.h>

// Bit-Access Functions
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

enum States {Init, S1, S2} state;
	
	unsigned char tmpA = 0x00; // LED sensor
	unsigned char tmpB = 0x00; // LED light toggle
	
void Tick()
{
	switch(state){ // State Transitions
		case Init:
		state = S1;
		break;
		case S1:
		if(tmpA) {
			state = S2;
		}
		else{
		state = S1;
		}
		break;
		
		case S2:
		if(tmpA) {
			state = S1;
		}
		else{
		state = S2;
		}
		break;
		
		default:
		state = Init;
		break;
	}
	
	switch(state){ // State Actions
		case Init:
		break;
		
		case S1:
		tmpB = SetBit(tmpB, 0, 1);
		tmpB = SetBit(tmpB, 1, 0);
		break;
		
		case S2:
		tmpB = SetBit(tmpB, 0, 0);
		tmpB = SetBit(tmpB, 1, 1);
		break;
		
		default:
		break;
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; // Configure Port A's all 8 pins as Inputs
    DDRB = 0xFF; PORTB = 0x00; // Configure Port B's 8 pins as Outputs, Initialize to 0s.
    
    tmpB = SetBit(tmpB, 0, 1);
    state = Init;
    
    // Start execution here
    while (1) 
    {
		tmpA = PINA;
		Tick();
		PORTB = tmpB;		
    }
}

