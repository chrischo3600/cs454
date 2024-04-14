/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: simple HelloWorld application     */
/*                for Amazing Ball platform         */
/*                                                  */
/****************************************************/

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


int main(){
    __C30_UART=1;	
	lcd_initialize();
    led_initialize();
	lcd_clear();
	lcd_locate(0,0);
	lcd_printf_d("-- Lab 06 --");
    
    uint16_t x_samples[5];
    uint16_t y_samples[5];
    
    uint16_t corner_x[4] = {SERVOS_LO, SERVOS_HI, SERVOS_HI, SERVOS_LO};
    uint16_t corner_y[4] = {SERVOS_LO, SERVOS_LO, SERVOS_HI, SERVOS_HI};
    int j;
    int i;
    
    
    
    while (1) {
        for (j = 0; j < 4; j++) {
            
            uint16_t check[5] = {2, 3, 4, 5, 1};
            // move
            set_motor_angle(CHAN_X, corner_x[j]);
            set_motor_angle(CHAN_Y, corner_y[j]);
           
            
            // sample x
            touch_init('x');
            touch_select_dim('x');
            for (i = 0; i < 5; i++) {
                x_samples[i] = touch_read();
            
            }
            
            // sample y
            __delay_ms(10);
            touch_init('y');
            touch_select_dim('y');

            for (i = 0; i < 5; i++) {
                y_samples[i] = touch_read();
            }
            
            // find median
            qsort(x_samples, 5, sizeof(x_samples[0]), compare);
            qsort(y_samples, 5, sizeof(y_samples[0]), compare);
            qsort(check, 5, sizeof(check[0]), compare);
               
            // prints
            lcd_locate(0, j+1);
            lcd_printf_d("C%d: X= %d, Y= %d ", j+1, x_samples[2], y_samples[2]);
            lcd_locate(0, 6);
            lcd_printf_d("%d, %d, %d, %d, %d", check[0], check[1], check[2], check[3], check[4]);
        }
        
    }
    
    return 0;
}


