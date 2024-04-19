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
#define LCD_MIN 1
// lcd_printf_d max
//#define LCD_MAX_X 20.0 
//#define LCD_MAX_Y 7.0
#define LCD_MAX_X 125.0
#define LCD_MAX_Y 61.0
#define TOUCH_MAX_X 2800.0
#define TOUCH_MIN_X 290.0
#define TOUCH_MAX_Y 2400.0
#define TOUCH_MIN_Y 370.0
#define TOUCH_MID_X 1545.0
#define TOUCH_MID_Y 1385.0
#define BALL_RADIUS 2
#define DOTS_LENGTH 20
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define BUTTER_ORD 3
#define X 0
#define KP 0.01
#define KD 0
#define KI 0


// butterworth ord 3
// Wn = (5)/(20/2)  # in hz filter
// fs = 20  # Sample rate in hz
// N = 3
//double b[4] = {0.00041655, 0.00124964, 0.00124964,  0.00041655};
//double a[4] = { 1.,        -2.6861574,  2.41965511, -0.73016535};
double b[4] = {0.01809893, 0.0542968,  0.0542968, 0.01809893};
double a[4] = { 1.,         -1.76004188,  1.18289326, -0.27805992};

int x_val;
int filter; 
double lastErrorX = 0; 
double sumError = 0;


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
    motor_init(channel);
    motor_set_duty(channel, us_value);
    __delay_ms(2000);
}

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}




void __attribute__((__interrupt__)) _T2Interrupt(void) {
    TMR2=0;
    x_val = touch_read();
    filter = 1;
    CLEARBIT(IFS0bits.T2IF);
}

int online_filter(double sampled_value, int * inputs, int * outputs) {
    double retval;
	int i;

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

double pid_controller(int x, double kp, double kd, double ki) {
    double errorX = x - TOUCH_MID_X;
    sumError += errorX;
    double errChange = errorX - lastErrorX;
    lastErrorX = errorX;
    lcd_locate(0, 2);
    lcd_printf_d("error: %f", errorX);
    
    // error
    double P = kp * errorX; 
    lcd_locate(0, 3);
    lcd_printf_d("P: %f", P);
    // rate of sampling in seconds
    double dt = 50.0/1000.0;
    
    // derivative of error 
    double D = kd * (errChange/dt);
    
    // integral of errors
    double I = ki * sumError;
    
    return P + D + I;
}





int main(){
    __C30_UART=1;	
	lcd_initialize();
    led_initialize();
	lcd_clear();
    int i;
    motor_init(CHAN_Y);
    set_motor_angle(CHAN_Y, SERVOS_LO);
    touch_init();
    int inputs[BUTTER_ORD+1] = {0, 0, 0, 0};
    int outputs[BUTTER_ORD+1] = {0, 0, 0, 0};
    
    
    // select x dimension
    motor_init(CHAN_X);
    touch_select_dim(X);
    
    //setup Timer 2
    __builtin_write_OSCCONL(OSCCONL | 2);
    CLEARBIT(T2CONbits.TON); // Disable Timer
    CLEARBIT(T2CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE); // Disable Gated Timer mode
    TMR2 = 0x00; // Clear timer register
    T2CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    PR2 = 2500; // 50000/(1000/50 ms)
    IPC1bits.T2IP = 0x01; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T2IF);
    SETBIT(IEC0bits.T2IE); // Enable Timer1 interrupt
    SETBIT(T2CONbits.TON); // Start Timer
    
    

    while(1) {
        if (filter) {
            x_val = online_filter(x_val, &inputs, &outputs);
            lcd_locate(0, 0);
            lcd_printf_d("x_val: %d    ", x_val);
            
            
            double pid = pid_controller(x_val, KP, KD, KI);
            pid += SERVOS_MEAN;
            lcd_locate(0, 1);
            lcd_printf_d("pid_val: %f     ", pid);
            set_motor_angle(CHAN_X, (int) pid);
            filter = 0;
        }
    
    }
    
    
}
