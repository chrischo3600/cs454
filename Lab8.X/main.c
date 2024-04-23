/*
 * File:   main.c
 * Author: team-7a
 *
 * Created on April 17, 2024, 3:06 PM
 */

#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>
#include <uart.h>
#include <xc.h>
#include "types.h"
#include "led.h"
#include "lcd.h"
#include "joystick.h"
#include <stdlib.h>
#define CHAN_X 8
#define CHAN_Y 7
#define SERVOS_LO 900
#define SERVOS_HI 2100
#define SERVOS_MEAN 1500
#define NUM_SAMPLES 5
#define TOUCH_LO_X 310
#define TOUCH_LO_Y 350
#define TOUCH_HI_X 2900
#define TOUCH_HI_Y 2500
//#define TOUCH_MID_X 3200.0
//#define TOUCH_MID_Y 2600.0

#define TOUCH_MID_X 1605.0
#define TOUCH_MID_Y 1425.0

#define BUTTER_ORD 3
#define X 0
#define Y 1

//#define KPx 0.001
//#define KDx 0.001
//#define KIx 0.000
//
//#define KPy 0.002
//#define KDy 0.0
//#define KIy 0.000

#define KPx 0.001
#define KDx 0.00
#define KIx 0.000

#define KPy 0.001
#define KDy 0.0
#define KIy 0.000



// butterworth ord 3
// Wn = (2)/(20/2)  # in hz filter
// N = 3
double b[4] = {0.01809893, 0.0542968,  0.0542968,  0.01809893};
double a[4] = { 1.       ,  -1.76004188,  1.18289326, -0.27805992};

//double b[4] = {0.01809893, 0.0542968,  0.0542968, 0.01809893};
//double a[4] = { 1.,         -1.76004188,  1.18289326, -0.27805992};

int val;
int filter; 
double lastErrorX = 0; 
double sumErrorX = 0;

int isX = 1;

double lastErrorY = 0; 
double sumErrorY = 0;

int samplesX[5] = {0, 0, 0, 0, 0};
int samplesY[5] = {0, 0, 0, 0, 0};

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

void set_motor_angle(uint8_t channel, uint16_t us_value) {
    motor_set_duty(channel, us_value);
}

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}


void __attribute__((__interrupt__)) _T3Interrupt(void) {
    TMR3=0;
    val = touch_read();
    filter = 1;
    CLEARBIT(IFS0bits.T3IF);
}

int online_filter(double sampled_value, int * inputs, int * outputs) {
    double retval;
	int i;
    int diff = sampled_value - inputs[0];
    
    if(diff > 1000) {
        sampled_value = inputs[0];
    }
    

	// Perform sample shift
	for (i = BUTTER_ORD; i > 0; --i) {
		inputs[i] = inputs[i-1];
		outputs[i] = outputs[i-1];
	}
	inputs[0] = sampled_value;

	// Compute filtered value
	retval = 0;
	for (i = 0; i < BUTTER_ORD+1; ++i) {
		retval += inputs[i] * b[i];
		if (i > 0)
			retval -= outputs[i] * a[i];
	}
	outputs[0] = retval;
   

	return retval;
    
}

double pid_controller_x(int x, double kp, double kd, double ki) {
    double errorX = x - TOUCH_MID_X;
    sumErrorX += errorX;
    double errChange = errorX - lastErrorX;
    lastErrorX = errorX;
    
    // error
    double P = kp * errorX; 
    // sampling rate in seconds 
    double dt = 50.0/1000.0;
    // derivative of error 
    double D = kd * (errChange/dt);
    
    // integral of errors
    double I = ki * sumErrorX;
    
    double pid = 0 - P - D - I;
    
    double pid_max = 0 -((TOUCH_LO_X - TOUCH_MID_X) * kp) - (1000/dt) * kd; //1300 * KP
    double pid_min = 0 -((TOUCH_HI_X - TOUCH_MID_X) * kp) - (-1*((1/2)*1000)/dt)*kd; // -1295 * kp
    
    double angle = (SERVOS_HI - SERVOS_LO)/(pid_max - pid_min) * (pid - pid_min) + SERVOS_LO;
    
    lcd_locate(0, 5);
    lcd_printf_d("%.1f, %.1f, %.1f, %.1f ", pid, angle, pid_min, pid_max);
    
    return angle;
}

double pid_controller_y(int y, double kp, double kd, double ki) {
    double errorY = y - TOUCH_MID_Y;
    sumErrorY += errorY;
    double errChange = errorY - lastErrorY;
    lastErrorY = errorY;
    
    // error
    double P = kp * errorY; 
    // rate of sampling in seconds
    double dt = 50.0/1000.0;
    
    // derivative of error 
    double D = kd * (errChange/dt);
    
    // integral of errors
    double I = ki * sumErrorY;
    
    double pid = 0 - P - D - I;
    
    double pid_max = 0 -((TOUCH_LO_Y - TOUCH_MID_Y) * kp) - (1000/dt) * kd;
    double pid_min = 0 -((TOUCH_HI_Y - TOUCH_MID_Y) * kp) - (-1*((1/2)*1000)/dt)*kd;
    
    double angle = (SERVOS_HI - SERVOS_LO)/(pid_max - pid_min) * (pid - pid_min) + SERVOS_LO;
    
    lcd_locate(0, 6);
    lcd_printf_d("%.1f, %.1f, %.1f, %.1f ", pid, angle, pid_min, pid_max);
    
    return angle;
}




int main(){
    __C30_UART=1;	
	lcd_initialize();
    led_initialize();
    motor_init();
    touch_init();
	lcd_clear();
    
    lcd_locate(0, 0);
    lcd_printf_d("--- X ---  --- Y ---");
    lcd_locate(0, 1);
    lcd_printf_d("KP: %.3f", KPx);
    lcd_locate(10, 1);
    lcd_printf_d("KP: %.3f", KPy);
    lcd_locate(0, 2);
    lcd_printf_d("KI: %.3f", KIx);
    lcd_locate(10, 2);
    lcd_printf_d("KI: %.3f", KIy);
    lcd_locate(0, 3);
    lcd_printf_d("KD: %.3f", KDx);
    lcd_locate(10, 3);
    lcd_printf_d("KD: %.3f", KDy);
    lcd_locate(0, 4);
    lcd_printf_d("LOC: ");
    lcd_locate(10, 4);
    lcd_printf_d("LOC: ");
    
    int i;
    motor_init(CHAN_Y);
    set_motor_angle(CHAN_Y, SERVOS_LO);
    
    
    int inputsX[BUTTER_ORD+1] = {2400, 2400, 2400, 2400};
    int outputsX[BUTTER_ORD+1] = {2400, 2400, 2400, 2400};
    
    int inputsY[BUTTER_ORD+1] = {2000, 2000, 2000, 2000};
    int outputsY[BUTTER_ORD+1] = {2000, 2000, 2000, 2000};
    
    //motor_switch(CHAN_Y);
    //set_motor_angle(CHAN_Y, SERVOS_HI);
    //__delay_ms(2000);
    //motor_switch(CHAN_X);
    //set_motor_angle(CHAN_X, SERVOS_HI);
    //__delay_ms(2000);
    
    
    
    
    int counter = 0;

    
    //setup Timer 2
    __builtin_write_OSCCONL(OSCCONL | 2);
    CLEARBIT(T3CONbits.TON); // Disable Timer
    CLEARBIT(T3CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T3CONbits.TGATE); // Disable Gated Timer mode
    TMR3 = 0x00; // Clear timer register
    T3CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    PR3 = 1250; // 50000/(1000/(50/2 ms)
    IPC2bits.T3IP = 0x01; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T3IF);
    SETBIT(IEC0bits.T3IE); // Enable Timer1 interrupt
    SETBIT(T3CONbits.TON); // Start Timer
    
    int filtered_valX;
    int filtered_valY;
    touch_select_dim(X);
    __delay_ms(10);
    
    while(1) {
        if (filter) {
            
            if (isX) {
                filtered_valX = online_filter(val, &inputsX, &outputsX);
                //lcd_locate(5, 4);
                double pid = pid_controller_x(filtered_valX, KPx, KDx, KIx);
                //pid += SERVOS_MEAN;
                //lcd_printf_d("%d,", (int) pid);
                isX = 0;
                touch_select_dim(Y);
                __delay_ms(10);
                //motor_switch(CHAN_Y);
                
            } else {
                filtered_valY = online_filter(val, &inputsY, &outputsY);
                //lcd_locate(15, 4);
                double pid = pid_controller_y(filtered_valY, KPy, KDy, KIy);
                //set_motor_angle(CHAN_X, (int)pid);
                //pid += SERVOS_MEAN;
                //lcd_printf_d("%d,", (int) pid);
                isX = 1;
                touch_select_dim(X);
                __delay_ms(10);
                //motor_switch(CHAN_X);
            }
                
               
            filter = 0;
            
        }
    
    }
    
    // SAMPLING AND FILTER
    while(1) {
        if (filter) {
            if (isX) {
                touch_select_dim(X);
                __delay_ms(10);
                val = touch_read();
                filtered_valX = online_filter(val, &inputsX, &outputsX);
                lcd_locate(0, 5);
                lcd_printf_d("x: %d, %d ", val, filtered_valX);
                isX = 0;
            } else {
                touch_select_dim(Y);
                __delay_ms(10);
                val = touch_read();
                filtered_valY = online_filter(val, &inputsY, &outputsY);
                lcd_locate(0, 6);
                lcd_printf_d("y: %d, %d ", val, filtered_valY);
                isX = 1;
                
            }
            filter = 0;
            
        }
    
    }
    
    
    // WITH MEDIAN
    int counterX = 0;
    int counterY = 0;
    while(1) {
        if (filter) {
            
            if (isX) {
                touch_select_dim(X);
                __delay_ms(10);
                if (counterX < 5) {
                    val = touch_read();
                    samplesX[counterX] = val;
                    counterX++;
                } else {
                    qsort(samplesX, 5, sizeof(samplesX[0]), compare);
                    motor_init(CHAN_X);
                    filtered_valX = online_filter(samplesX[0], &inputsX, &outputsX);
                    //lcd_locate(0, 4);
                    //lcd_printf_d("x: %d, %d ", samplesX[0], filtered_valX);

                    double pid = pid_controller_x(filtered_valX, KPx, KDx, KIx);
                    pid += SERVOS_MEAN;

                    set_motor_angle(CHAN_X, (int)pid);
                    counterX = 0;
                    isX = 0;
                }
                
                
                
            } else {
                touch_select_dim(Y);
                __delay_ms(10);
                if (counterY < 5) {
                    val = touch_read();
                    samplesY[counterY] = val;
                    counterY++;
                } else {
                    qsort(samplesY, 5, sizeof(samplesY[0]), compare);

                    motor_init(CHAN_Y);
                    filtered_valY = online_filter(samplesY[0], &inputsY, &outputsY);
                    //lcd_locate(0, 5);
                    //lcd_printf_d("y: %d, %d ", samplesY[0], filtered_valY);

                    double pid = pid_controller_y(filtered_valY, KPy, KDy, KIy);
                    pid += SERVOS_MEAN;
                    set_motor_angle(CHAN_Y, (int)pid);
                    counterY = 0;
                    isX = 1;
                }
                
                
            }
            filter = 0;
            
        }
    
    }
    
    
}
