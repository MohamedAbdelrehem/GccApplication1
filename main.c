/*
 * GccApplication1.c
 *
 * Created: 11/21/2022 2:52:04 PM
 * Author : Mohamed
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <math.h>

#include <util/delay.h>

//main

//lab7

#define N0  8 //prescaler
#define F_WAVE  5000 // 5khz					//when N=1==OCR0=799==f=5000
#define OCR0_VALUE ((F_CPU/2/N0/F_WAVE)-1)		// Check datasheet for this equation

void init_timer0_CTC();//generate sqare wave 5khz


//if needed add global variables (use volatile to avoid compiler optimization)

volatile unsigned int x =0;
int main(void)
{

	// disable interrupts (clear I bit in SREG)
	cli();

	// assign pins PC0:6 as output

	DDRC = 0xFF;
	PORTC = 0b001111111; //0x00;		//led pins		<<< output not input

	//Initialize Timer 0
	init_timer0_CTC();//CTC

	//enable interrupts
	sei();

	while (1){


	} ;
}


void init_timer0_CTC()
{
	//this function configures the generated signal at OC0

	//initialize TCNTC0 = 0	
	TCNT0 = 0;
	TCCR0 = 0;	//initialize the Timer/Counter Control Register
	//set OCR0 value
	//#define OCR0_VALUE ((F_CPU/2/N0/F_WAVE)-1)				//OCR0= OCR0_VALUE;

	// choose CTC mode															   bit 3    bit 7
	TCCR0|=(1<<WGM01);	//activate CTC from table page 80 in Datasheet Table 14-2. WGM01=1 WGM00=0	intialy zero
	
	//  Toggle OC0 on compare match					//COM01 COM00
	TCCR0|=(1<<COM00);								//  =0    =1 

	// , Prescaler 8								//CS02 CS01 CS00
	TCCR0|=(1<<CS01);								//=0   =1   =0
													
									
	DDRB=0xFF;
	PORTB=0x08; //pin 3 for oscilloscope	

	// Make PB3 (OC0) as output pin
	OCR0= OCR0_VALUE;

	// Enable output compare interrupt
	TIMSK|=(1<<OCIE0);	/// TOIE0 timer overflow flag << what is the diff		(last bit in TIMSK – Timer/Counter Interrupt Mask Register) datasheet
	//OCIE0 timer counter commpate = Timer/Counter0 Compare Match Interrupt Enable
}



ISR(TIMER0_COMP_vect) // using TIMER0 CTC to toggle leds besides generating square wave
{

	//write code to toggle the leds on PC0:6 every 1s
	x++;
	if (x > 1000){	//let x = 0 to start waiting for another 5kHz to light up again
		x=0;
		PORTC=~PORTC;
	}
}
