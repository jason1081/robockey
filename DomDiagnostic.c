/*
 *  DomDiagonistic.c
 *  
 *
 *  Created by Nick Pesta on 12/1/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */


#include "m_general.h"
#include "m_bus.h"
#include "m_port.h"
#include "m_rf.h"
#include "m_usb.h"
#include <math.h>	
#include "m_wii.h"
#include "stdlib.h"

#define RXADDRESS 0x34
#define LENGTH 12
#define CHANNEL 1
#define PLAY 0xA1
#define GOALA 0xA2
#define PAUSE 0xA4
#define COMM 0xA0
#define PUCKFIND 10
#define GOTOGOAL 11

void init_port(void)
{
	m_port_init(0x20); // Initializes the port expander and sets all of the G pins as outputs
	m_port_set(0x20, DDRG,0);	
	m_port_set(0x20, DDRG,1);
	m_port_set(0x20, DDRG,2);
	m_port_set(0x20, DDRG,3);
	m_port_set(0x20, DDRG,4);
	m_port_set(0x20, DDRG,5);
	m_port_set(0x20, DDRG,6);
	m_port_set(0x20, DDRG,7);
}
void clearDisplay(void)
{	m_port_clear(0x20, PORTG,0); //Clear Display
	m_port_clear(0x20, PORTG,1);
	m_port_clear(0x20, PORTG,2);
	m_port_clear(0x20, PORTG,3);
	m_port_clear(0x20, PORTG,4);
	m_port_clear(0x20, PORTG,5);
	m_port_clear(0x20, PORTG,6);
}
void disp0(void)
{
	m_port_set(0x20, PORTG,0); //Display 0
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void disp1(void)
{
	m_port_set(0x20, PORTG,5); //Display 1
	m_port_set(0x20, PORTG,6);
	
}
void disp2(void)
{	m_port_set(0x20, PORTG,1);//Display 2
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
}
void disp3(void)
{
	m_port_set(0x20, PORTG,1); //Display 3
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void disp4(void)
{
	m_port_set(0x20, PORTG,0); //Display 4
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void disp5(void)
{
	m_port_set(0x20, PORTG,0); //Display 5
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,6);
}
void disp6(void)
{
	m_port_set(0x20, PORTG,0); //Display 6
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,6);
}
void disp7(void)
{
	m_port_set(0x20, PORTG,4); //Display 7
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void disp8(void)
{
	m_port_set(0x20, PORTG,0); //Display 8
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void disp9(void)
{
	m_port_set(0x20, PORTG,0); //Display 9
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispA(void)
{
	m_port_set(0x20, PORTG,0); //Display A
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispC(void)
{
	m_port_set(0x20, PORTG,0); //Display C
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
}
void dispD(void)
{
	 //Display d
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispE(void)
{		
	m_port_set(0x20, PORTG,0); //Display E
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
}
void dispF(void)
{
	m_port_set(0x20, PORTG,0); //Display F
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,4);
	
}
void dispG(void)
{
	m_port_set(0x20, PORTG,0); //Display g
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispH(void)
{
	m_port_set(0x20, PORTG,0); //Display 8
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispI(void)
{	m_port_set(0x20, PORTG,0); //Display 8
	m_port_set(0x20, PORTG,2);
}
void dispJ(void)
{
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispL(void)
{
	m_port_set(0x20, PORTG,0); //Display L
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
}
void dispN(void)
{
	 //Display n
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,6);
}
void dispO(void)
{		
	m_port_set(0x20, PORTG,1); //Display o
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,6);
}
void dispP(void)
{
	m_port_set(0x20, PORTG,0); //Display P
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
}
void dispU(void)
{
	m_port_set(0x20, PORTG,0); //Display U
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}
void dispY(void)
{
	m_port_set(0x20, PORTG,0); //Display Y
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
}

void initTimer(void) {
    /*
     *  Initialize timer 
     */
    OCR1A = 31999;
    OCR1B = 15999;
    OCR1C = 15999;

    set(TCCR1B, WGM13); // Mode 15: up to OCR1A, PWM, single slope
    set(TCCR1B, WGM12); // ^
    set(TCCR1A, WGM11); // ^
    set(TCCR1A, WGM10); // ^

    set(TCCR1A, COM1B1); //clear at OCR1B, set at rollover
    clear(TCCR1A, COM1B0); // ^

    set(TCCR1A, COM1C1); // clear at OCR1C, set at rollover
    clear(TCCR1A, COM1C0);

    clear(TCCR1B, CS12); // set clock pre-scaler to /1
    clear(TCCR1B, CS11); // ^
    set(TCCR1B, CS10); // ^

    /*
     *  Initialize motors 
     */
    set(DDRB, 6);
    set(DDRB, 7);
    set(DDRB, 5);
    set(DDRB, 3);
}

int main(void)
{	m_clockdivide(0);
	m_bus_init();
	
	
	init_port();
        initTimer();
	
	clearDisplay();

	
	while(1)
	{
		set(PORTB,6); // Left wheel
		set(PORTB,7); // Right wheel
		set(PORTB,5); // Left wheel forward if set 
		set(PORTB,3); // Right wheel forward if set
//		m_red(ON);
//		m_wait(500);
//		m_red(OFF);
//		m_green(ON);
//		m_wait(500);
//		m_green(OFF);
//		disp0();
//		m_wait(500);
//		clearDisplay();
//		disp1();
//		m_wait(500);
//		clearDisplay();
//		disp2();
//		m_wait(500);
//		clearDisplay();
//		disp3();
//		m_wait(500);
//		clearDisplay();
//		disp4();
//		m_wait(500);
//		clearDisplay();
//		disp5();
//		m_wait(500);
//		clearDisplay();
//		disp6();
//		m_wait(500);
//		clearDisplay();
//		disp7();
//		m_wait(500);
//		clearDisplay();
//		disp8();
//		m_wait(500);
//		clearDisplay();
//		disp9();
//		m_wait(500);
//		clearDisplay();
//		dispA();
//		m_wait(500);
//		clearDisplay();
//		dispC();
//		m_wait(500);
//		clearDisplay();
//		dispD();
//		m_wait(500);
//		clearDisplay();
//		dispE();
//		m_wait(500);
//		clearDisplay();
//		dispF();
//		m_wait(500);
//		clearDisplay();
//		dispG();
//		m_wait(500);
//		clearDisplay();
//		dispH();
//		m_wait(500);
//		clearDisplay();
//		dispI();
//		m_wait(500);
//		clearDisplay();
//		dispJ();
//		m_wait(500);
//		clearDisplay();
//		dispL();
//		m_wait(500);
//		clearDisplay();
//		dispN();
//		m_wait(500);
//		clearDisplay();
//		dispO();
//		m_wait(500);
//		clearDisplay();
//		dispP();
//		m_wait(500);
//		clearDisplay();
//		dispU();
//		m_wait(500);
//		clearDisplay();
//		dispY();
//		m_wait(500);
//		clearDisplay();
//		m_wait(4000);
		
	}
}	
