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

volatile int StarA[2] = {0, 0};
volatile int StarB[2] = {0, 0};
volatile int StarC[2] = {0, 0};
volatile int StarD[2] = {0, 0};
int OffsetX = 0, OffsetY = 0;
//int Rot_Off[2] = {0,0};
int a = 0, low = -90, high = 90, b = 0, c = 0, d = 0; j = 7;

volatile unsigned char buffer[LENGTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // Buffer for the wireless transmissions
volatile int state = 0x00;  
volatile unsigned int blobs[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // Buffer for the wii stars data
unsigned int x_1 = 0, x_2 = 0, x_3 = 0, x_4 = 0, y_1 = 0, y_2 = 0, y_3 = 0, y_4 = 0; // Integers to store the star locations in x and y coordinates

int OneTwo = 0, OneThree = 0, OneFour = 0, TwoThree = 0, TwoFour = 0, ThreeFour = 0, big = 0, next = 0;
int D[6] = {0, 0, 0, 0, 0, 0};
int Center[2] = {512, 384};
int AvgX = 0, AvgY = 0, XRobot = 0, YRobot = 0;
double Theta = 0, changeX = 0, changeY = 0, deltax = 0, deltay = 0;

double coordinates[3] = {0,0,0};
// B6 & B7 are enable lines
// B5 & B3 are direction lines
double pi = 3.1415926;
double x_target = 0, y_target = 0;
double target_angle = 0, theta_target = 0, theta_rotate = 0, theta_target_d = 0, Theta_d = 0, theta_rotate_d = 0;
double tol = 2;


void clearDisplay(void)
{	m_port_clear(0x20, PORTG,0); //Clear Display
	m_port_clear(0x20, PORTG,1);
	m_port_clear(0x20, PORTG,2);
	m_port_clear(0x20, PORTG,3);
	m_port_clear(0x20, PORTG,4);
	m_port_clear(0x20, PORTG,5);
	m_port_clear(0x20, PORTG,6);
}
void commtestf(void)
{	clearDisplay();
	
	m_port_set(0x20, PORTG,0); //Display 4
	m_port_set(0x20, PORTG,2);
	m_port_set(0x20, PORTG,3);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,5);
	m_port_set(0x20, PORTG,6);
	state = 0x00;
	
}
void pausef(void)
{	clearDisplay();
	
	m_port_set(0x20, PORTG,0); //Display 4
	m_port_set(0x20, PORTG,1);
	m_port_set(0x20, PORTG,4);
	m_port_set(0x20, PORTG,6);
	state = 0x00;
	
	OCR1B = 0;
	OCR1C = 0;
	while(1){
		sei();
	}
	
}

void playf(void)
{ 	clearDisplay();
	m_port_set(0x20, PORTG,4); //Display 1
	m_port_set(0x20, PORTG,6);
	
	
	state = 0x00;
}


int main(void)
{	m_clockdivide(0);
	m_bus_init();
	m_usb_init();
	m_rf_open(CHANNEL,RXADDRESS,LENGTH);
	m_wii_open();
	
	OCR1A = 31999;
	OCR1B = 0;
	OCR1C = 0;
	
	set(TCCR1B,WGM13);          // Mode 15: up to OCR1A, PWM, single slope
	set(TCCR1B,WGM12);          // ^
	set(TCCR1A,WGM11);          // ^
	set(TCCR1A,WGM10);          // ^
	
	set(TCCR1A,COM1B1);			//clear at OCR1B, set at rollover
	clear(TCCR1A,COM1B0);			// ^
	
	set(TCCR1A,COM1C1); // clear at OCR1C, set at rollover
	clear(TCCR1A,COM1C0);
	
	clear(TCCR1B,CS12);       // set clock pre-scaler to /1
	clear(TCCR1B,CS11);          // ^
	set(TCCR1B,CS10);         // ^
	
	
	m_port_init(0x20); // Initializes the port expander and sets all of the G pins as outputs
	m_port_set(0x20, DDRG,0);	
	m_port_set(0x20, DDRG,1);
	m_port_set(0x20, DDRG,2);
	m_port_set(0x20, DDRG,3);
	m_port_set(0x20, DDRG,4);
	m_port_set(0x20, DDRG,5);
	m_port_set(0x20, DDRG,6);
	m_port_set(0x20, DDRG,7);
	
	clearDisplay();
	// Set motor output ports
	set(DDRB,6);
	set(DDRB,7);
	set(DDRB,5);
	set(DDRB,3);
	
	// Turn off motors
	//clear(PORTB,6);
	//	clear(PORTB,7);
	
	while(1){
		//switch(state){
//			case COMM:
//				commtestf();
//				break;
//			case PAUSE:
//				pausef();
//				break;
//			case PLAY:
//				playf();
//				m_usb_tx_int(4);
//				motorcontrolf();
//				break;
//			default:
//				break;
//		
	//localizef();
	//m_wait(500);
		
	}
	
}


ISR(INT2_vect){
	if(m_rf_read(buffer,LENGTH)){
		m_green(TOGGLE);
		state = buffer[0];}
	switch(state){
		case COMM:
			commtestf();
			break;
		case PAUSE:
			pausef();
			break;
		case PLAY:
			playf();
			//m_usb_tx_int(4);
			motorcontrolf();
			break;
		default:
			break;
	}
	
	
}


void localizef(void)
{
	
	//long PtoC = .30851; //converts pixels to centimeters
	
	
	//	if x(1) ~= 1023 &&  x(2) ~= 1023 && x(3) ~= 1023 && x(4) ~= 1023 && y(1) ~= 1023 && y(2) ~= 1023 && y(3) ~= 1023 && y(4) ~= 1023 
	m_wii_read(blobs);
	x_1 = blobs[0];
	y_1 = blobs[1];
	x_2 = blobs[3];
	y_2 = blobs[4];
	x_3 = blobs[6];
	y_3 = blobs[7];
	x_4 = blobs[9];
	y_4 = blobs[10];
	
	if( (x_1 == 1023 && y_1 == 1023) ||(x_2 == 1023 && y_2 == 1023) || (x_3 == 1023 && y_3 == 1023) ||(x_4 == 1023 && y_4 == 1023)){
	return;
	}
	
	OneTwo=sqrt(((x_2-x_1)*(x_2-x_1))+((y_2-y_1)*(y_2-y_1)));//Find distances to all possible stars
	OneThree=sqrt(((x_3-x_1)*(x_3-x_1))+((y_3-y_1)*(y_3-y_1)));
	OneFour=sqrt(((x_4-x_1)*(x_4-x_1))+((y_4-y_1)*(y_4-y_1)));
	TwoThree=sqrt(((x_3-x_2)*(x_3-x_2))+((y_3-y_2)*(y_3-y_2)));
	TwoFour=sqrt(((x_4-x_2)*(x_4-x_2))+((y_4-y_2)*(y_4-y_2)));
	ThreeFour=sqrt(((x_4-x_3)*(x_4-x_3))+((y_4-y_3)*(y_4-y_3)));
	
	
	D[0] = OneTwo;
	D[1] = OneThree;
	D[2] = OneFour;
	D[3] = TwoThree;
	D[4] = TwoFour;
	D[5] = ThreeFour;
	// a = [d1, d2, d3, d4, d5, d6];
    // O(n) = n;
	int i = 0;
	big = 0;
	next = 0;
    for(i=0;i<6;i++){
        if(D[i]>big){
            next=big;
            big=D[i];
        }
        else if (D[i] > next && D[i] != big){
            next = D[i];
        }
	}
    
	//	m_usb_tx_uint(D[0]);
	//	m_usb_tx_string(" ");
	
	//----------------------------------------1
	if (big == OneTwo){
		if(next == TwoThree){
			StarA[0] = x_1;
			StarA[1] = y_1;
			StarB[0] = x_3;
			StarB[1] = y_3;
			StarC[0] = x_2;
			StarC[1] = y_2;
			StarD[0] = x_4;
			StarD[1] = y_4;
		}
		if (next == TwoFour) {
			StarA[0] = x_1;
			StarA[1] = y_1;
			StarB[0] = x_4;
			StarB[1] = y_4;
			StarC[0] = x_2;
			StarC[1] = y_2;
			StarD[0] = x_3;
			StarD[1] = y_3;
		}
		if (next == OneThree){
			StarA[0] = x_2;
			StarA[1] = y_2;
			StarB[0] = x_3;
			StarB[1] = x_3;
			StarC[0] = x_1;
			StarC[1] = y_1;
			StarD[0] = x_4;
			StarD[1] = y_4;
		}
		if (next == OneFour){
			StarA[0] = x_2;
			StarA[1] = y_2;
			StarB[0] = x_4;
			StarB[1] = y_4;
			StarC[0] = x_1;
			StarC[1] = y_1;
			StarD[0] = x_3;
			StarD[1] = y_3;
			
		}
	}
	//-------------------------------------2
	if (big == OneThree){
		if (next == TwoThree){
			StarA[0] = x_1;
			StarA[1] = y_1;
			StarB[0] = x_2;
			StarB[1] = y_2;
			StarC[0] = x_3;
			StarC[1] = y_3;
			StarD[0] = x_4;
			StarD[1] = y_4;
		}
		if (next == ThreeFour){
			StarA[0] = x_1;
			StarA[1] = y_1;
			StarB[0] = x_4;
			StarB[1] = y_4;
			StarC[0] = x_3;
			StarC[1] = y_3;
			StarD[0] = x_2;
			StarD[1] = y_2;
		}
		if (next == OneTwo){
			StarA[0] = x_3;
			StarA[1] = y_3;
			StarB[0] = x_2;
			StarB[1] = y_2;
			StarC[0] = x_1;
			StarC[1] = y_1;
			StarD[0] = x_4;
			StarD[1] = y_4;
		}
		if (next == OneFour){
			StarA[0] = x_3;
			StarA[1] = y_3;
			StarB[0] = x_4;
			StarB[1] = y_4;
			StarC[0] = x_1;
			StarC[1] = y_1;
			StarD[0] = x_2;
			StarD[1] = y_2;
		}
	}
	//-------------------------------------3
	if (big == OneFour){
		if (next == ThreeFour){
			StarA[0] = x_1;
			StarA[1] = y_1;
			StarB[0] = x_3;
			StarB[1] = y_3;
			StarC[0] = x_4;
			StarC[1] = y_4;
			StarD[0] = x_2;
			StarD[1] = y_2;
		}
		if (next == TwoFour){
			StarA[0] = x_1;
			StarA[1] = y_1;
			StarB[0] = x_2;
			StarB[1] = y_2;
			StarC[0] = x_4;
			StarC[1] = y_4;
			StarD[0] = x_3;
			StarD[1] = y_3;
		}
		if (next == OneThree){
			StarA[0] = x_4;
			StarA[1] = y_4;
			StarB[0] = x_3;
			StarB[1] = y_3;
			StarC[0] = x_1;
			StarC[1] = y_1;
			StarD[0] = x_2;
			StarD[1] = y_2;
		}    
		if (next == OneTwo){
			StarA[0] = x_4;
			StarA[1] = y_4;
			StarB[0] = x_2;
			StarB[1] = y_2;
			StarC[0] = x_1;
			StarC[1] = y_1;
			StarD[0] = x_3;
			StarD[1] = y_3;
		} 
	}
	//-------------------------------------4
	if (big == TwoThree){
		if (next == OneThree){
			StarA[0] = x_2;
			StarA[1] = y_2;
			StarB[0] = x_1;
			StarB[1] = y_1;
			StarC[0] = x_3;
			StarC[1] = y_3;
			StarD[0] = x_4;
			StarD[1] = y_4;
		}
		if (next == ThreeFour){
			StarA[0] = x_2;
			StarA[1] = y_2;
			StarB[0] = x_4;
			StarB[1] = y_4;
			StarC[0] = x_3;
			StarC[1] = y_3;
			StarD[0] = x_1;
			StarD[1] = y_1;
		}
		if (next == OneTwo){
			StarA[0] = x_3;
			StarA[1] = y_3;
			StarB[0] = x_1;
			StarB[1] = y_1;
			StarC[0] = x_2;
			StarC[1] = y_2;
			StarD[0] = x_4;
			StarD[1] = y_4;
		}    
		if (next == TwoFour){
			StarA[0] = x_3;
			StarA[1] = y_3;
			StarB[0] = x_4;
			StarB[1] = y_4;
			StarC[0] = x_2;
			StarC[1] = y_2;
			StarD[0] = x_1;
			StarD[1] = y_1;
		} 
	}
	//-------------------------------------5
	if (big == TwoFour){
		if (next == ThreeFour){
			StarA[0] = x_2;
			StarA[1] = y_2;
			StarB[0] = x_3;
			StarB[1] = y_3;
			StarC[0] = x_4;
			StarC[1] = y_4;
			StarD[0] = x_1;
			StarD[1] = y_1;
		}
		if (next == OneFour){
			StarA[0] = x_2;
			StarA[1] = y_2;
			StarB[0] = x_1;
			StarB[1] = y_1;
			StarC[0] = x_4;
			StarC[1] = y_4;
			StarD[0] = x_3;
			StarD[1] = y_3;
		}
		if (next == TwoThree){
			StarA[0] = x_4;
			StarA[1] = y_4;
			StarB[0] = x_3;
			StarB[1] = y_3;
			StarC[0] = x_2;
			StarC[1] = y_2;
			StarD[0] = x_1;
			StarD[1] = y_1;
		}    
		if (next == OneTwo){
			StarA[0] = x_4;
			StarA[1] = y_4;
			StarB[0] = x_1;
			StarB[1] = y_1;
			StarC[0] = x_2;
			StarC[1] = y_2;
			StarD[0] = x_3;
			StarD[1] = y_3;
		} 
	}
	//-------------------------------------6
	if (big == ThreeFour){
		if (next == OneFour){
			StarA[0] = x_3;
			StarA[1] = y_3;
			StarB[0] = x_1;
			StarB[1] = y_1;
			StarC[0] = x_4;
			StarC[1] = y_4;
			StarD[0] = x_2;
			StarD[1] = y_2;
		}
		if (next == TwoFour){
			StarA[0] = x_3;
			StarA[1] = y_3;
			StarB[0] = x_2;
			StarB[1] = y_2;
			StarC[0] = x_4;
			StarC[1] = y_4;
			StarD[0] = x_1;
			StarD[1] = y_1;
		}
		if (next == OneThree){
			StarA[0] = x_4;
			StarA[1] = y_4;
			StarB[0] = x_1;
			StarB[1] = y_1;
			StarC[0] = x_3;
			StarC[1] = y_3;
			StarD[0] = x_2;
			StarD[1] = y_2;
		}    
		if (next == TwoThree){
			StarA[0] = x_4;
			StarA[1] = y_4;
			StarB[0] = x_2;
			StarB[1] = y_2;
			StarC[0] = x_3;
			StarC[1] = y_3;
			StarD[0] = x_1;
			StarD[1] = y_1;
		} 
	}
	//-----------------------FIND OFFSET--------------------------------------
	AvgX = (StarA[0] + StarC[0])/2;
	AvgY = (StarA[1] + StarC[1])/2;
	
	OffsetX = AvgX - Center[0];
	OffsetY = AvgY - Center[1];
	
	//------------------------FIND THE ROTATION-------------------------------
	changeX = StarC[0]  - StarA[0];
	changeY = StarC[1] - StarA[1];
	
	Theta = atan2(changeY,changeX);
	
	Theta_d = Theta*57.296;
	
	XRobot = (OffsetY*cos(Theta) - OffsetX*sin(Theta)); //Converts the offset from the frame of the robot to the frame of the playing field
	YRobot = (OffsetY*sin(Theta) + OffsetX*cos(Theta));
		
}

void motorcontrolf(void)
{	
	sei();
	
	localizef();
	
	if (XRobot < 0) {
		x_target = 280;
		y_target = 0 ;
		
		clearDisplay();
		//		m_port_set(0x20, PORTG,1); //Display 2
		//		m_port_set(0x20, PORTG,2);
		//		m_port_set(0x20, PORTG,3);
		//		m_port_set(0x20, PORTG,5);
		//		m_port_set(0x20, PORTG,6);
	}
	else {
		x_target = -280;
		y_target = 0;
		clearDisplay();
		//		m_port_set(0x20, PORTG,1); //Display 3
		//		m_port_set(0x20, PORTG,3);
		//		m_port_set(0x20, PORTG,4);
		//		m_port_set(0x20, PORTG,5);
		//		m_port_set(0x20, PORTG,6);
	}
	
	deltay = y_target-YRobot;
	deltax = x_target-XRobot;
	
	theta_target = atan2(deltay, deltax)+pi;
	if (theta_target > pi) {
		theta_target = theta_target - 2*pi;
	}
	theta_target_d = theta_target*57.296;
	
	//theta_rotate = theta_target - Theta;
	theta_rotate_d = theta_target_d - Theta_d;

	//m_usb_tx_int(Theta_d);
	//m_usb_tx_string(" ");
	//m_usb_tx_int(theta_target_d);
	//m_usb_tx_string(" ");
	//m_usb_tx_int(theta_rotate_d);
	//m_usb_tx_string(" ");
	//m_usb_tx_int(XRobot);
	//m_usb_tx_string(" ");
	//m_usb_tx_int(YRobot);
	//m_usb_tx_string("\n");

	// Determine motor directions
	if (theta_rotate_d < 0) {
		// Turn clockwise
		
		set(PORTB,5);
		clear(PORTB,3);
		
		clearDisplay();
		m_port_set(0x20, PORTG,0); //Display 5
		m_port_set(0x20, PORTG,1);
		m_port_set(0x20, PORTG,3);
		m_port_set(0x20, PORTG,4);
		m_port_set(0x20, PORTG,5);
	} else {
		// Turn counter-clockwise
		
		clear(PORTB,5);
		set(PORTB,3);
		clearDisplay();
		m_port_set(0x20, PORTG,0); //Display 6
		m_port_set(0x20, PORTG,1);
		m_port_set(0x20, PORTG,2);
		m_port_set(0x20, PORTG,3);
		m_port_set(0x20, PORTG,4);
	}
	
	// Turn motor until current angle matches target angle
	while (Theta_d > theta_target_d + tol || Theta_d < theta_target_d - tol) {
		sei();
		OCR1B = 4990;
		OCR1C = 4990;
		localizef();
		clearDisplay();
		m_port_set(0x20, PORTG,4); //Display 7
		m_port_set(0x20, PORTG,5);
		m_port_set(0x20, PORTG,6);
	}
	
	clearDisplay();
	
	// Move bot forward
	OCR1B = 16000;
	OCR1C = 16000;
	set(PORTB,5);
	set(PORTB,3);
	
	// Turn motor until current coordinates match target coordinates
	if(x_target < 0){
		while (XRobot > x_target) { // ADD TOLERANCE *******
			sei();
			m_wait(8);
			localizef();
		}
	}else{
		while (XRobot < x_target) { // ADD TOLERANCE *******
			sei();
			m_wait(8);
			localizef();
		}
	}
	
	// Stop motors
	OCR1B = 0;
	OCR1C = 0;
	clear(PORTB,6);
 	clear(PORTB,7);	
	clearDisplay();
	m_port_set(0x20, PORTG,4); //Display 1
	m_port_set(0x20, PORTG,6);
}