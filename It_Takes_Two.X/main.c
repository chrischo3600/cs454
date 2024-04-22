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
#define BALL_RADIUS 2
#define DOTS_LENGTH 20
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


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

void get_dots(int* dots_arr, int max) {
    int i;
    for (i = 0; i < DOTS_LENGTH; i++) {
        dots_arr[i] = rand() % max;
    }
    
}

int touch_to_lcd_x(uint16_t touch_x, int radius) {
   int lcd_x = (LCD_MAX_X - LCD_MIN)/(TOUCH_MAX_X - TOUCH_MIN_X) * (touch_x - TOUCH_MIN_X) + LCD_MIN;
   // adjust for circle radius
   lcd_x = MIN(lcd_x, LCD_MAX_X);
   return lcd_x;
}

int touch_to_lcd_y(uint16_t touch_y, int radius) {
   int lcd_y = (LCD_MAX_Y - LCD_MIN)/(TOUCH_MAX_Y - TOUCH_MIN_Y) * (touch_y - TOUCH_MIN_Y) + LCD_MIN;
   // adjust for circle radius
   
   lcd_y = MIN(lcd_y, LCD_MAX_Y);
   return lcd_y;
}

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

void print_dots(int* dots_x, int* dots_y) {
    int i;
    for (i = 0; i < DOTS_LENGTH; i++) {
        drawCircle(dots_x[i], dots_y[i], 1, 1);
    }
}


int main(){
    __C30_UART=1;	
	lcd_initialize();
    led_initialize();
	lcd_clear();
    uint16_t x_samples[NUM_SAMPLES];
    uint16_t y_samples[NUM_SAMPLES];
    int i;
    set_motor_angle(CHAN_X, SERVOS_MEAN);
    set_motor_angle(CHAN_X, SERVOS_MEAN);
    touch_init();
    uint16_t prev_x = 0;
    uint16_t prev_y = 0;
    int dots_x[DOTS_LENGTH];
    int dots_y[DOTS_LENGTH];
    
    get_dots(&dots_x, LCD_MAX_X);
    get_dots(&dots_y, LCD_MAX_Y);
    print_dots(&dots_x, &dots_y);
    
    while(1);
    
    while(1) {
        
        
        touch_select_dim(0);
        __delay_ms(100);
        for (i = 0; i < NUM_SAMPLES; i++) {
            x_samples[i] = touch_read();
        }
        
        // sample y
        touch_select_dim(1);
        __delay_ms(100);
        for (i = 0; i < NUM_SAMPLES; i++) {
            y_samples[i] = touch_read();
        }
        
        qsort(x_samples, 5, sizeof(x_samples[0]), compare);
        qsort(y_samples, 5, sizeof(y_samples[0]), compare);
        
        int lcd_x = touch_to_lcd_x(x_samples[2], BALL_RADIUS);
        int lcd_y = touch_to_lcd_y(y_samples[2], BALL_RADIUS);
        
        //drawCircle(prev_x, prev_y, 4, 0);
        //drawCircle(lcd_x, lcd_y, 2, 1);
        prev_x = lcd_x;
        prev_y = lcd_y;
        
    }
    
}
