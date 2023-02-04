/*
 * mgalo001_lab3_part3.c
 *
 * Created: 4/8/2019 12:51:21 PM
 * Author : Micah Galos - mgalo001@ucr.edu
 * Partner(s) Name & E-mail: Ronny Anariva - ranar001@ucr.edu
 * Lab Section: 027
 * Assignment: Lab 2 Exercise 3
 * Exercise Description: In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated, and PA6 is 1
						 if the driver's seatbelt is fastened. PC7 should light a "Fasten seatbelt" icon if a key is in the ignition, the
						 driver is seated, but the belt is not fastened.
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */

#include <avr/io.h>

// Function Accessors: Bit-access function
/*unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}*/
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, set to 0
	
	unsigned char tmpA0 = 0x00; // Initialize PA0 to 0 
	unsigned char tmpA1 = 0x00; // Initialize PA1 to 0
	unsigned char tmpC = 0x00; // Initialize PC to 0
	
	while (1)
	{
		tmpA0 = PINA & 0x0F; // Read input for fuel levels 
		tmpA1 = PINA & 0x70; // Read input for PA4, PA5, and PA6
		
		if(tmpA0 == 1 || tmpA0 == 2){ // tmpC is PC5 [0x20]
			tmpC = 0x20;
		}
		if(tmpA0 == 3 || tmpA0 == 4){ // tmpC is PC5-PC4 [0x30]
			tmpC = 0x30;
		}
		if(tmpA0 == 5 || tmpA0 == 6){ // tmpC is PC5-PC3 [0x38]
			tmpC = 0x38;
		}
		if(tmpA0 >= 7 && tmpA0 <= 9){ // tmpC is PC5-PC2 [0x3C]
			tmpC = 0x3C;
		}						
		if(tmpA0 >= 10 && tmpA0 <= 12){ // tmpC is PC5-PC1 [0x3E]
			tmpC = 0x3E;
		}
		if(tmpA0 >= 13 && tmpA0 <= 15){ // tmpC is PC5-PC0 [0x3F]
			tmpC = 0x3F;
		}
		if(tmpA0 < 4){ // tmpC is PA6 [0x40]
			tmpC += 0x40;
		}		
		// PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated, and PA6 is 0 if seatbelt is not fastened
		if(!GetBit(tmpA1, 6) && GetBit(tmpA1, 5) && GetBit(tmpA1, 4)){
			tmpC += 0x80;
		}
		else{
			tmpC = tmpC;
		}		
		PORTC = tmpC; // Outputs the resulting sensor
		tmpC = 0x00; // Resets the value from result
	}
}
