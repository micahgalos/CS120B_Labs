/*
 * mgalo001_lab11_part5.c
 *
 * Created: 5/8/2019 1:32:22 AM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 11 Exercise 5
 * Exercise Description: Using both rows of the LCD display, design a game where a player controlled character avoids oncoming obstacles.
 *						 Three buttons are used to operate the game. 
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "io.c"
#include "scheduler.h"

#define button1 (~PINC & 0x01)
#define button2 (~PINC & 0x02)
#define button3 (~PINC & 0x04)

unsigned char tempB = 0x00;
unsigned char i = 9;
unsigned char j = 29;
unsigned char cursor = 17;
unsigned char pause = 1;
unsigned char lose = 0;

enum SM1_States{SM1_run, SM1_lose};
enum SM2_States{SM2_wait, SM2_run, SM2_down, SM2_up, SM2_pause_press, SM2_pause_release};

int SMTick1(int state){
	switch(state){
		case SM1_run:
		if(!pause){
			if(cursor == 1 && i == 1) {
				state = SM1_lose;
				LCD_DisplayString(7, "You            Lose!");
				lose = 1;
			}
			else if(cursor == 17 && j == 17) {
				state = SM1_lose;
				LCD_DisplayString(7, "You            Lose!");
				lose = 1;
			}
			else {
				LCD_ClearScreen();
				LCD_Cursor(i);
				LCD_WriteData('#');
				LCD_Cursor(j);
				LCD_WriteData('#');
				i--;
				j--;
				if(i==0) {
					i = 16;
				}
				if(j==16) {
					j=32;
				}
			}
		}
		
		break;
		case SM1_lose:
		if(button1) {
			state = SM1_run;
			cursor = 17;
			pause = 1;
			lose = 0;
			i = 9;
			j = 29;
		}
		else
		state = SM1_lose;
		break;
	}
	return state;
}

int SMTick2(int state){
	switch(state){
		case SM2_wait:
		if(button1) {
			state = SM2_pause_press;
		}
		else if(!lose && pause == 0)
		state = SM2_run;
		else
		state = SM2_wait;
		break;
		case SM2_run:
		if(button1) {
			state = SM2_pause_press;
		}
		else if(button2) {
			state = SM2_down;
		}
		else if(button3) {
			state = SM2_up;
		}
		else if(lose) {
			state = SM2_wait;
		}
		else {
			state = SM2_run;
		}
		
		break;
		case SM2_up:
		state = SM2_run;
		break;
		case SM2_down:
		state = SM2_run;
		break;
		case SM2_pause_press:
		if(!button1) {
			state = SM2_pause_release;
		}
		else {
			state = SM2_pause_press;
		}
		break;
		case SM2_pause_release:
		state = SM2_wait;
		break;
		default:
		break;
	}
	switch(state){
		case SM2_wait:
		LCD_Cursor(17);
		break;
		case SM2_run:
		LCD_Cursor(cursor);
		break;
		case SM2_up:
		cursor = 1;
		break;
		case SM2_down:
		cursor = 17;
		break;
		case SM2_pause_press:
		break;
		case SM2_pause_release:
		if(pause == 1)
		pause = 0;
		else
		pause = 1;
		break;
		default:
		break;
	}
	return state;
}


int main()
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	
	// Period for the tasks
	unsigned long int SMTick1_calc = 200;
	unsigned long int SMTick2_calc = 10;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;
	
	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;

	//Declare an array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = 0;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.
	
	// Task 2
	task2.state = 0;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &SMTick2;//Function pointer for the tick.

	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	LCD_init();
	
	LCD_Cursor(i);
	LCD_WriteData('#');
	LCD_Cursor(j);
	LCD_WriteData('#');
	
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

