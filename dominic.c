// DOM
// The left motor is controlled by B6 (enable) and B5 (direction
// The right motor is controlled by B7 and B3
// Both motors go forward when the direction ports are set high

#include "m_general.h"
#include "m_bus.h"
#include "m_port.h"
#include "m_rf.h"
#include "m_usb.h"
#include <math.h>
#include "m_wii.h"
#include "stdlib.h"
#include <avr/wdt.h>

#define RXADDRESS 0x34
#define LENGTH 12
#define CHANNEL 1
#define PLAY 0xA1
#define GOALA 0xA2
#define PAUSE 0xA4
#define COMM 0xA0
#define PUCKFIND 10
#define GOTOGOAL 11
#define GOALA 0xA2
#define GOALB 0xA3
#define HALFTIME 0xA6
#define GAMEOVER 0xA7
#define REVERSE 12

volatile int StarA[2] = {0, 0};
volatile int StarB[2] = {0, 0};
volatile int StarC[2] = {0, 0};
volatile int StarD[2] = {0, 0};
int OffsetX = 0, OffsetY = 0;
int a = 0, low = -90, high = 90, b = 0, c = 0, d = 0, j = 7, i = 0, wiggle = 20;
int reverse_count, should_reverse;

volatile unsigned char buffer[LENGTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Buffer for the wireless transmissions
volatile int state = 0x00;
volatile unsigned int blobs[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // Buffer for the wii stars data
unsigned int x_1 = 0, x_2 = 0, x_3 = 0, x_4 = 0, y_1 = 0, y_2 = 0, y_3 = 0, y_4 = 0; // Integers to store the star locations in x and y coordinates

int OneTwo = 0, OneThree = 0, OneFour = 0, TwoThree = 0, TwoFour = 0, ThreeFour = 0, big = 0, next = 0;
int D[6] = {0, 0, 0, 0, 0, 0};
int Center[2] = {512, 384};
int AvgX = 0, AvgY = 0, XRobot = 0, YRobot = 0, XRobot_old, YRobot_old;
double Theta = 0, changeX = 0, changeY = 0, deltax = 0, deltay = 0;
int photo1 = 0, photo2 = 0, photo3 = 0, photo4 = 0, photo5 = 0, high1 = 800, high2 = 800, high3 = 800, high4 = 800, high5 = 0, port = 0; //Variables for the ADC converions involved in the puck finding

float pi = 3.1415926;
double x_target = 0, y_target = 0;
double target_angle = 0, theta_target = 0, theta_rotate = 0, theta_target_d = 0, Theta_d = 0, theta_rotate_d = 0;
double tol = 6;

void clearDisplay(void) {
    m_port_clear(0x20, PORTG, 0); //Clear Display
    m_port_clear(0x20, PORTG, 1);
    m_port_clear(0x20, PORTG, 2);
    m_port_clear(0x20, PORTG, 3);
    m_port_clear(0x20, PORTG, 4);
    m_port_clear(0x20, PORTG, 5);
    m_port_clear(0x20, PORTG, 6);
}

void disp0(void) {
    m_port_set(0x20, PORTG, 0); //Display 0
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void disp1(void) {
    m_port_set(0x20, PORTG, 5); //Display 1
    m_port_set(0x20, PORTG, 6);
}

void disp2(void) {
    m_port_set(0x20, PORTG, 1); //Display 2
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
}

void disp3(void) {
    m_port_set(0x20, PORTG, 1); //Display 3
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void disp4(void) {
    m_port_set(0x20, PORTG, 0); //Display 4
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void disp5(void) {
    m_port_set(0x20, PORTG, 0); //Display 5
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 6);
}

void disp6(void) {
    m_port_set(0x20, PORTG, 0); //Display 6
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 6);
}

void disp7(void) {
    m_port_set(0x20, PORTG, 4); //Display 7
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void disp8(void) {
    m_port_set(0x20, PORTG, 0); //Display 8
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void disp9(void) {
    m_port_set(0x20, PORTG, 0); //Display 9
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispA(void) {
    m_port_set(0x20, PORTG, 0); //Display A
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispC(void) {
    m_port_set(0x20, PORTG, 0); //Display C
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
}

void dispD(void) {
    //Display d
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispE(void) {
    m_port_set(0x20, PORTG, 0); //Display E
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
}

void dispF(void) {
    m_port_set(0x20, PORTG, 0); //Display F
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 4);
}

void dispG(void) {
    m_port_set(0x20, PORTG, 0); //Display g
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispH(void) {
    m_port_set(0x20, PORTG, 0); //Display 8
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispI(void) {
    m_port_set(0x20, PORTG, 0); //Display 8
    m_port_set(0x20, PORTG, 2);
}

void dispJ(void) {
    m_port_set(0x20, PORTG, 3); //Display J
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispL(void) {
    m_port_set(0x20, PORTG, 0); //Display L
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
}

void dispN(void) {
    m_port_set(0x20, PORTG, 1); //Display n
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 6);
}

void dispO(void) {
    m_port_set(0x20, PORTG, 1); //Display o
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 6);
}

void dispP(void) {
    m_port_set(0x20, PORTG, 0); //Display P
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 4);
    m_port_set(0x20, PORTG, 5);
}

void dispU(void) {
    m_port_set(0x20, PORTG, 0); //Display U
    m_port_set(0x20, PORTG, 2);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void dispY(void) {
    m_port_set(0x20, PORTG, 0); //Display Y
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 3);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
}

void printADC(void) {
    m_usb_tx_int(photo1);
    m_usb_tx_string("\t");
    m_usb_tx_int(photo2);
    m_usb_tx_string("\t");
    m_usb_tx_int(photo3);
    m_usb_tx_string("\t");
    m_usb_tx_int(photo4);
    m_usb_tx_string("\t");
    m_usb_tx_int(photo5);
    m_usb_tx_string("\n");
}

void printPos(void) {
    localizef();
    m_usb_tx_int(XRobot);
    m_usb_tx_string("\t");
    m_usb_tx_int(YRobot);
    m_usb_tx_string("\t");
    m_usb_tx_int(Theta_d);
    m_usb_tx_string("\n");
}

void initADC(void) {
    clear(ADMUX, REFS1); // Setting the reference voltage for the ADC to Vcc
    set(ADMUX, REFS0); // ^
    
    set(ADCSRA, ADPS2); // set ADC pre-scaler to 128
    set(ADCSRA, ADPS1); // ^
    set(ADCSRA, ADPS0); //^
    
    set(DIDR0, ADC0D); // disable digital on F0 (ADC0)
    set(DIDR0, ADC1D); // disable digital to F1
    set(DIDR0, ADC4D); //disable digital to F4
    set(DIDR0, ADC5D); //disable digital to F5
    set(DIDR0, ADC6D); //disable digital to F6
    set(DIDR0, ADC7D); //disable digital to F7
    
    set(ADMUX, REFS0); // set ADC reference to Vcc
    
    set(ADCSRA, ADIE); //Enables interrupts when conversion finishes
    //	clear(ADMUX,MUX2); // This is the front right. Range 50-1023. Port is F1
    //	set(ADMUX,MUX0);
    
    set(ADCSRA, ADEN); // enable ADC subsystem
    set(ADCSRA, ADSC); // start ADC conversion
}

void initPort(void) {
    m_port_init(0x20); // Initializes the port expander and sets all of the G pins as outputs
    m_port_set(0x20, DDRG, 0);
    m_port_set(0x20, DDRG, 1);
    m_port_set(0x20, DDRG, 2);
    m_port_set(0x20, DDRG, 3);
    m_port_set(0x20, DDRG, 4);
    m_port_set(0x20, DDRG, 5);
    m_port_set(0x20, DDRG, 6);
    m_port_set(0x20, DDRG, 7);
}

void initTimer(void) {
    /*
     *  Initialize timer
     */
    OCR1A = 31999;
    OCR1B = 0;
    OCR1C = 0;
    
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

void commtestf(void) {
    clearDisplay();
    dispH();
    m_wait(100);
    clearDisplay();
    m_wait(100);
    dispE();
    m_wait(100);
    clearDisplay();
    m_wait(100);
    dispL();
    m_wait(100);
    clearDisplay();
    m_wait(100);
    dispL();
    m_wait(100);
    clearDisplay();
    m_wait(100);
    disp0();
    m_wait(100);
    clearDisplay();
    state = 0x00;
}

void pausef(void) {
    clearDisplay();
    
    m_port_set(0x20, PORTG, 0); //Display 4
    m_port_set(0x20, PORTG, 1);
    m_port_set(0x20, PORTG, 5);
    m_port_set(0x20, PORTG, 6);
    state = 0x00;
    
    OCR1B = 0;
    OCR1C = 0;
    clear(PORTB,6);
    clear(PORTB,7);
}

void playf(void) {
    clearDisplay();
    dispP();
    state = PUCKFIND;
}

int main(void) {
    m_clockdivide(0);
    m_bus_init();
    m_usb_init();
    m_rf_open(CHANNEL, RXADDRESS, LENGTH);
    m_wii_open();
    m_disableJTAG();
    sei();
    initTimer();
    initPort();
    clearDisplay();
    initADC();
    
    wdt_reset();
    wdt_enable(WDTO_1S);
    
    localizef();
    if (XRobot < 0) {
        x_target = 350;
        y_target = 0;
        
        clearDisplay();
        dispA();
    } else {
        x_target = -350;
        y_target = 0;
        clearDisplay();
        dispC();
    }
        
    //playf(); // Debug
    while (1) {
        //state = GOTOGOAL; // Debug
        //localizef();
        //printPos(); // Debug
        //printADC(); // Debug
        /*
        // Increment reverse counter if displacement is small
        localizef();
        if (abs(XRobot - XRobot_old) < 5 && abs(YRobot - YRobot_old) < 5) {
            should_reverse++;
        }
        else {
            should_reverse = 0;
        }
        // Determine whether to reverse
        if (should_reverse > 15 && state != PAUSE && state != HALFTIME && state != GOALA && state != GOALB && state != GAMEOVER) {
            state = REVERSE;
        }
        */
        // Change action based on state
        switch (state) {
            case COMM:
                commtestf();
                break;
            case PAUSE:
                pausef();
                break;
            case PLAY:
                playf();
                break;
            case PUCKFIND:
                puckfindf();
                high5 = 350;
                if(state != PAUSE && state != HALFTIME && state != GOALA && state != GOALB && state != GAMEOVER)
                {
                    if (photo5 > high5)
                    {
                        state = GOTOGOAL;
                    }
                }
                break;
            case GOTOGOAL:
                gotogoalf();
                high5 = 330;
                if(state != PAUSE && state != HALFTIME && state != GOALA && state != GOALB && state != GAMEOVER)
                {
                    if (photo5 < high5) {
                        state = PUCKFIND;
                    }
                }
                break;
//	    case REVERSE:
//		reversef();
//		break;
            case HALFTIME:
                pausef();
                break;
            case GOALA:
                pausef();
                break;
            case GOALB:
                pausef();
                break;
            case GAMEOVER:
                pausef();
                break;
            default:
                break;
        }
        m_red(TOGGLE);
        m_wait(30);
        wdt_reset();
    }
}

ISR(ADC_vect) {
    switch (port) {
        case 0:
            clear(ADMUX, MUX2); // This is the front right. Range 50-1023. Port is F1
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0);
            photo5 = ADC; // For some reason, this is actually setting the value of side right
            port = 1;
            ADC = 0;
            break;
        case 1:
            set(ADMUX, MUX2); // This is the side right. Range 379-950. Port is F4
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0); // This is the side right. Range 350-950. Port is F4
            photo1 = ADC; // For some reason, this is actually setting the value of front right
            port = 2;
            ADC = 0;
            break;
        case 2:
            set(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0); // This is the side left. Range 150-950. Port is F5
            photo2 = ADC;
            ADC = 0;
            port = 3;
            break;
        case 3:
            set(ADMUX, MUX1); // This is the front left side. Port is F6
            set(ADMUX, MUX2);
            clear(ADMUX, MUX0);
            photo3 = ADC;
            ADC = 0;
            port = 4;
            break;
        case 4:
            set(ADMUX, MUX1); // This is the possesion one. Port is F7
            set(ADMUX, MUX2);
            set(ADMUX, MUX0);
            photo4 = ADC;
            ADC = 0;
            port = 0;
    }
    set(ADCSRA, ADSC);
}

ISR(INT2_vect) {
    if (m_rf_read(buffer, LENGTH)) {
        m_green(TOGGLE);
        state = buffer[0];
    }
}

void localizef(void) {
    
    //long PtoC = .30851; //converts pixels to centimeters
    
    m_wii_read(blobs);
    
    //    if ((blobs[0] != 1023) && (blobs[1] != 1023)) {
    //        x_1 = blobs[0];
    //        y_1 = blobs[1];
    //    }
    //    if ((blobs[3] != 1023) && (blobs[4] != 1023)) {
    //        x_2 = blobs[3];
    //        y_2 = blobs[4];
    //    }
    //    if ((blobs[6] != 1023) && (blobs[7] != 1023)) {
    //        x_3 = blobs[6];
    //        y_3 = blobs[7];
    //    }
    //    if ((blobs[9] != 1023) && (blobs[10] != 1023)) {
    //        x_4 = blobs[9];
    //        y_4 = blobs[10];
    //    }
    
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
    
    OneTwo = sqrt(((x_2 - x_1)*(x_2 - x_1))+((y_2 - y_1)*(y_2 - y_1))); //Find distances to all possible stars
    OneThree = sqrt(((x_3 - x_1)*(x_3 - x_1))+((y_3 - y_1)*(y_3 - y_1)));
    OneFour = sqrt(((x_4 - x_1)*(x_4 - x_1))+((y_4 - y_1)*(y_4 - y_1)));
    TwoThree = sqrt(((x_3 - x_2)*(x_3 - x_2))+((y_3 - y_2)*(y_3 - y_2)));
    TwoFour = sqrt(((x_4 - x_2)*(x_4 - x_2))+((y_4 - y_2)*(y_4 - y_2)));
    ThreeFour = sqrt(((x_4 - x_3)*(x_4 - x_3))+((y_4 - y_3)*(y_4 - y_3)));
    
    
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
    for (i = 0; i < 6; i++) {
        if (D[i] > big) {
            next = big;
            big = D[i];
        } else if (D[i] > next && D[i] != big) {
            next = D[i];
        }
    }
    
    //----------------------------------------1
    if (big == OneTwo) {
        if (next == TwoThree) {
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
        if (next == OneThree) {
            StarA[0] = x_2;
            StarA[1] = y_2;
            StarB[0] = x_3;
            StarB[1] = x_3;
            StarC[0] = x_1;
            StarC[1] = y_1;
            StarD[0] = x_4;
            StarD[1] = y_4;
        }
        if (next == OneFour) {
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
    if (big == OneThree) {
        if (next == TwoThree) {
            StarA[0] = x_1;
            StarA[1] = y_1;
            StarB[0] = x_2;
            StarB[1] = y_2;
            StarC[0] = x_3;
            StarC[1] = y_3;
            StarD[0] = x_4;
            StarD[1] = y_4;
        }
        if (next == ThreeFour) {
            StarA[0] = x_1;
            StarA[1] = y_1;
            StarB[0] = x_4;
            StarB[1] = y_4;
            StarC[0] = x_3;
            StarC[1] = y_3;
            StarD[0] = x_2;
            StarD[1] = y_2;
        }
        if (next == OneTwo) {
            StarA[0] = x_3;
            StarA[1] = y_3;
            StarB[0] = x_2;
            StarB[1] = y_2;
            StarC[0] = x_1;
            StarC[1] = y_1;
            StarD[0] = x_4;
            StarD[1] = y_4;
        }
        if (next == OneFour) {
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
    if (big == OneFour) {
        if (next == ThreeFour) {
            StarA[0] = x_1;
            StarA[1] = y_1;
            StarB[0] = x_3;
            StarB[1] = y_3;
            StarC[0] = x_4;
            StarC[1] = y_4;
            StarD[0] = x_2;
            StarD[1] = y_2;
        }
        if (next == TwoFour) {
            StarA[0] = x_1;
            StarA[1] = y_1;
            StarB[0] = x_2;
            StarB[1] = y_2;
            StarC[0] = x_4;
            StarC[1] = y_4;
            StarD[0] = x_3;
            StarD[1] = y_3;
        }
        if (next == OneThree) {
            StarA[0] = x_4;
            StarA[1] = y_4;
            StarB[0] = x_3;
            StarB[1] = y_3;
            StarC[0] = x_1;
            StarC[1] = y_1;
            StarD[0] = x_2;
            StarD[1] = y_2;
        }
        if (next == OneTwo) {
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
    if (big == TwoThree) {
        if (next == OneThree) {
            StarA[0] = x_2;
            StarA[1] = y_2;
            StarB[0] = x_1;
            StarB[1] = y_1;
            StarC[0] = x_3;
            StarC[1] = y_3;
            StarD[0] = x_4;
            StarD[1] = y_4;
        }
        if (next == ThreeFour) {
            StarA[0] = x_2;
            StarA[1] = y_2;
            StarB[0] = x_4;
            StarB[1] = y_4;
            StarC[0] = x_3;
            StarC[1] = y_3;
            StarD[0] = x_1;
            StarD[1] = y_1;
        }
        if (next == OneTwo) {
            StarA[0] = x_3;
            StarA[1] = y_3;
            StarB[0] = x_1;
            StarB[1] = y_1;
            StarC[0] = x_2;
            StarC[1] = y_2;
            StarD[0] = x_4;
            StarD[1] = y_4;
        }
        if (next == TwoFour) {
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
    if (big == TwoFour) {
        if (next == ThreeFour) {
            StarA[0] = x_2;
            StarA[1] = y_2;
            StarB[0] = x_3;
            StarB[1] = y_3;
            StarC[0] = x_4;
            StarC[1] = y_4;
            StarD[0] = x_1;
            StarD[1] = y_1;
        }
        if (next == OneFour) {
            StarA[0] = x_2;
            StarA[1] = y_2;
            StarB[0] = x_1;
            StarB[1] = y_1;
            StarC[0] = x_4;
            StarC[1] = y_4;
            StarD[0] = x_3;
            StarD[1] = y_3;
        }
        if (next == TwoThree) {
            StarA[0] = x_4;
            StarA[1] = y_4;
            StarB[0] = x_3;
            StarB[1] = y_3;
            StarC[0] = x_2;
            StarC[1] = y_2;
            StarD[0] = x_1;
            StarD[1] = y_1;
        }
        if (next == OneTwo) {
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
    if (big == ThreeFour) {
        if (next == OneFour) {
            StarA[0] = x_3;
            StarA[1] = y_3;
            StarB[0] = x_1;
            StarB[1] = y_1;
            StarC[0] = x_4;
            StarC[1] = y_4;
            StarD[0] = x_2;
            StarD[1] = y_2;
        }
        if (next == TwoFour) {
            StarA[0] = x_3;
            StarA[1] = y_3;
            StarB[0] = x_2;
            StarB[1] = y_2;
            StarC[0] = x_4;
            StarC[1] = y_4;
            StarD[0] = x_1;
            StarD[1] = y_1;
        }
        if (next == OneThree) {
            StarA[0] = x_4;
            StarA[1] = y_4;
            StarB[0] = x_1;
            StarB[1] = y_1;
            StarC[0] = x_3;
            StarC[1] = y_3;
            StarD[0] = x_2;
            StarD[1] = y_2;
        }
        if (next == TwoThree) {
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
    AvgX = (StarA[0] + StarC[0]) / 2;
    AvgY = (StarA[1] + StarC[1]) / 2;
    
    
    
    OffsetX = AvgX - Center[0];
    OffsetY = AvgY - Center[1];
    
    
    //------------------------FIND THE ROTATION-------------------------------
    changeX = StarC[0] - StarA[0];
    changeY = StarC[1] - StarA[1];
    
    Theta = atan2(changeY, changeX);
    
    Theta_d = Theta * 57.296;
    
    // Store old X, Y values
    XRobot_old = XRobot;
    YRobot_old = YRobot;
    
    // Save new X, Y values
    XRobot = (OffsetY * cos(Theta) - OffsetX * sin(Theta)); //Converts the offset from the frame of the robot to the frame of the playing field
    YRobot = (OffsetY * sin(Theta) + OffsetX * cos(Theta));
    
//    m_usb_tx_int(XRobot);
//    m_usb_tx_string(" ");
//    m_usb_tx_int(YRobot);
//    m_usb_tx_string("\n");
    
}

void gotogoalf(void) {
    clearDisplay();
    dispY();
    sei();
    
    localizef();
    deltay = y_target - YRobot;
    deltax = x_target - XRobot;
    
    theta_target = atan2(deltay, deltax) + pi;
    
    if (theta_target > pi) {
        theta_target = theta_target - 2 * pi;
    }
    theta_target_d = theta_target * 57.296;
    
    if ((Theta_d > (theta_target_d + tol)) || (Theta_d < (theta_target_d - tol))) {
        // If facing wrong direction, turn
        m_green(ON);
        //theta_rotate = theta_target - Theta;
        theta_rotate_d = theta_target_d - Theta_d;
        
        /* Determine motor directions:
         * -360 to -180: turn left
         * -180 to  0: turn right
         * 0 to 180: turn left
         * 180 to 360: turn right
         */
        if (theta_rotate_d < 0 && theta_rotate_d > -180
            || theta_rotate_d > 180) {
            // Turn right (CW)
            set(PORTB, 5);
            set(PORTB, 3);
            OCR1B = 8000;
            OCR1C = 16000;
        } else {
            // Turn left (CCW)
            set(PORTB, 5);
            set(PORTB, 3);
            OCR1B = 16000;
            OCR1C = 8000;
        }
    } else {
        m_green(OFF);
        // Facing correct direction, move bot forward
        set(PORTB, 5);
        set(PORTB, 3);
        
        OCR1B = 30000;
        OCR1C = 30000;
    }
}
/*
void reversef(void) {
    sei();
    dispU();
    
    if (reverse_count >= 20) {
        reverse_count=0;
        should_reverse=0;
        state = PUCKFIND;
    }
    else {
        clear(PORTB, 5);
        clear(PORTB, 3);
        OCR1B = 30000;
        OCR1C = 30000;
        reverse_count++;
    }
}
*/
void puckfindf(void) { // Photo1 and 4 are the phototransistor on the front of the bot. 2 and 3 are on the sides. 5 is the one that detects when we have possession of the puck
    sei();
    
    clearDisplay();
    disp1();
    
    if ((photo1 > photo2 && photo4 > photo3) && (photo1 < photo4 + wiggle && photo1 > photo4 - wiggle)) //Between front two phototransistors
    {
        clearDisplay();
        disp2();
        set(PORTB, 5);
        set(PORTB, 3);
        OCR1B = 30000;
        OCR1C = 30000;
    }
    else {
        
        if ((photo1 > photo2) && (photo4 > photo3) && (photo1 > photo4)) // slightly off to the right
        {
            clearDisplay();
            disp3();
            clear(PORTB, 5);//Right
            set(PORTB, 3);
            OCR1B = 6000;
            OCR1C = 6000;
        }
        else if ((photo1 > photo2) && (photo4 > photo3) && (photo1 < photo4)) //slightly off to the left
        {
            clearDisplay();
            disp8();
            set(PORTB, 5);
            clear(PORTB, 3);
            OCR1B = 6000;
            OCR1C = 6000;
        }
        
        if ((photo3 > photo4) && (photo3 > photo1) && (photo3 > photo2)) //way off to the left
        {
            clearDisplay();
            disp6();
            set(PORTB, 5);
            clear(PORTB, 3);
            OCR1B = 10000; //5500;
            OCR1C = 10000; //5500;
        }
        else if ((photo2 > photo4) && (photo2 > photo1) && (photo2 > photo3)) //way off to the right
        {
            clearDisplay();
            disp5();
            clear(PORTB, 5);
            set(PORTB, 3);
            OCR1B = 10000; //5500;
            OCR1C = 10000; //5500;
        
        }
        else{
            clearDisplay();
            disp9();
            set(PORTB, 5);
            set(PORTB, 3);
            OCR1B = 20000; //5500;
            OCR1C = 20000; //5500;
            m_green(TOGGLE);
        }
        
    }
}
