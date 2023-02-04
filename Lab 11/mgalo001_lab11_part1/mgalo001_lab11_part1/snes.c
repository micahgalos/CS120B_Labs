/*
 * snes.c
 *
 * Created: 5/24/2019 12:26:10 PM
 *  Author: Mike Galos
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "snes.h"
#include "bit.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

#define PORT PORTA
#define PIN PINA
#define LATCH  1 // Output
#define CLOCK  2 // Output
#define DATA   0 // Input

// SNES Controller Data Format: 0-15
// B, Y, SELECT, START, UP, DOWN, LEFT, RIGHT, A, X, L, R, 1, 1, 1, 1

unsigned short get_snesData(){
	unsigned short data = 0x0000;
	
	PORT = SetBit(PIN, LATCH, 1);
	_delay_us(12);
	PORT = SetBit(PIN, LATCH, 0);
	_delay_us(6);

	for (unsigned char i = 0; i < 12; i++){
		if(!GetBit(PIN, DATA)){
			data |= (1<<i);
		}
		else{
			data &= ~(1<<i);
		}
		
		PORT = SetBit(PIN, CLOCK, 1);
		_delay_us(6);
		PORT = SetBit(PIN, CLOCK, 0);
		_delay_us(6);
	}
	return data;
}