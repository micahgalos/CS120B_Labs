/*
 * mgalo001_lab11_part4.c
 *
 * Created: 5/8/2019 1:28:47 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 11 Exercise 3
 * Exercise Description: Notice that you can visually see the LCD refresh each character (display a lengthy string then update to a 
 *						 different lengthy string). Design a system where a single character is updated in the displayed string rather 
 *						 than the entire string itself. Use the functions provided in “io.c”.
 *
 *					     An example behavior would be to initially display a lengthy string, such as “Congratulations!”. The first 
 *						 keypad button pressed changes the first character ‘C’ to the button pressed. The second keypad press changes 
 *						 the second character to the second button pressed, etc. No refresh should be observable during the character 
 *						 update.
 *
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "io.c"
#include "keypad.h"
#include "scheduler.h"

unsigned char tempB = 0x00;
unsigned char count = 1;

enum SM1_States{SM1_output};

int SMTick1(int state){
	unsigned char x;
	x = GetKeypadKey();
	LCD_Cursor(count);
	switch(state){
		case SM1_output:
		switch (x) {
			case '1': tempB = 0x01;
			LCD_WriteData(tempB + '0'); count++; break;
			case '2': tempB = 0x02;
			LCD_WriteData(tempB + '0'); count++; break;
			case '3': tempB = 0x03;
			LCD_WriteData(tempB + '0'); count++; break;
			case '4': tempB = 0x04;
			LCD_WriteData(tempB + '0'); count++; break;
			case '5': tempB = 0x05;
			LCD_WriteData(tempB + '0'); count++; break;
			case '6': tempB = 0x06;
			LCD_WriteData(tempB + '0'); count++; break;
			case '7': tempB = 0x07;
			LCD_WriteData(tempB + '0'); count++; break;
			case '8': tempB = 0x08;
			LCD_WriteData(tempB + '0'); count++; break;
			case '9': tempB = 0x09;
			LCD_WriteData(tempB + '0'); count++; break;
			case 'A': tempB = 0x0A;
			LCD_WriteData(tempB + 0x37); count++; break;
			case 'B': tempB = 0x0B;
			LCD_WriteData(tempB + 0x37); count++; break;
			case 'C': tempB = 0x0C;
			LCD_WriteData(tempB + 0x37); count++; break;
			case 'D': tempB = 0x0D;
			LCD_WriteData(tempB + 0x37); count++; break;
			case '*': tempB = 0x0E;
			LCD_WriteData(tempB + 0x1C); count++; break;
			case '0': tempB = 0x00;
			LCD_WriteData(tempB + '0'); count++; break;
			case '#': tempB = 0x0F;
			LCD_WriteData(tempB + 0x14); count++; break;
			default: break;
		}
		if(count==17){
			count=1;
		}
		state = SM1_output;
		PORTB = tempB;
		break;
	}
	return state;
}


int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRD = 0xFF; PORTD = 0x00;
	// Period for the tasks
	unsigned long int SMTick1_calc = 200;

	//Calculating GCD
	unsigned long int tmpGCD = 1;

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc;

	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = 0;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.


	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	LCD_init();
	LCD_DisplayString(1,"Congratulations!");

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime >= tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

