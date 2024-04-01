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



/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

void init_joystick_axis_x() {
    // turn ADC OFF
    
    CLEARBIT(AD2CON1bits.ADON);
    
    // Select Port B Pin RB 4 to Analog
    SETBIT(TRISBbits.TRISB4);
    CLEARBIT(AD2PCFGLbits.PCFG4);
    
    // use 10-bit 1 channel
    CLEARBIT(AD2CON1bits.AD12B);
    // integer
    CLEARBIT(AD2CON1bits.FORM0);
    CLEARBIT(AD2CON1bits.FORM1);
    // automatic conversion
    SETBIT(AD2CON1bits.SSRC0);
    SETBIT(AD2CON1bits.SSRC1);
    SETBIT(AD2CON1bits.SSRC2);
    
    // set scanning sample to 0
    AD2CON2 = 0;
    
    // Conversion clock source
    CLEARBIT(AD2CON3bits.ADRC); // internal clock source
    AD2CON3bits.SAMC = 0x1F;    // sample-to-conversion clock = 31Tad
    AD2CON3bits.ADCS = 0x2;     // Tad = 3Tcy (Time cycles)
    
    // enable ADC
    SETBIT(AD2CON1bits.ADON);
    
}

void init_joystick_axis_y() {
    // turn ADC OFF
    
    CLEARBIT(AD2CON1bits.ADON);
    
    // Select Port B Pin RB 5 to Analog
    SETBIT(TRISBbits.TRISB5);
    CLEARBIT(AD2PCFGLbits.PCFG5);
    
    // use 10-bit 1 channel
    CLEARBIT(AD2CON1bits.AD12B);
    // integer
    CLEARBIT(AD2CON1bits.FORM0);
    CLEARBIT(AD2CON1bits.FORM1);
    // automatic conversion
    SETBIT(AD2CON1bits.SSRC0);
    SETBIT(AD2CON1bits.SSRC1);
    SETBIT(AD2CON1bits.SSRC2);
    
    // set scanning sample to 0
    AD2CON2 = 0;
    
    // Conversion clock source
    CLEARBIT(AD2CON3bits.ADRC); // internal clock source
    AD2CON3bits.SAMC = 0x1F;    // sample-to-conversion clock = 31Tad
    AD2CON3bits.ADCS = 0x2;     // Tad = 3Tcy (Time cycles)
    
    // enable ADC
    SETBIT(AD2CON1bits.ADON);
    
}


int main(){
	/* LCD Initialization Sequence */
    int debounceVal = 1;
	__C30_UART=1;	
	lcd_initialize();
    led_initialize();
	lcd_clear();
	lcd_locate(0,0);
	lcd_printf_d("-- Lab 05 --");
    
    init_joystick_axis_x();
    /// buttons
    AD1PCFGHbits.PCFG20 = 1;
    TRISEbits.TRISE8 = 1; // setting it as input
    
    INTCON2bits.INT1EP = 1;
    TRISDbits.TRISD10 = 1;
    
    
    
    uint16_t ADCValue;
    uint16_t max_x;
    uint16_t min_x;
    uint16_t min_y;
    uint16_t max_y;
    int go = 1;
    uint16_t T = 0;
    uint8_t pressed = 0;
    uint16_t released = 0;
    
    while(go) {
        AD2CHS0bits.CH0SA = 4;
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        CLEARBIT(AD2CON1bits.DONE);
        ADCValue = ADC2BUF0;
        lcd_locate(0,1);
        lcd_printf_d("JS Max x: %d    ", ADCValue);
   
        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;
            
            if (T > debounceVal) {
                if (pressed == 0) {
                    // pressed
                    max_x = ADCValue;
                    go = 0;
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > debounceVal) {
                pressed = 0;
            }
        } 
        
        
    }
    
    go = 1;
    T = 0;
    pressed = 0;
    released = 0;
    
    while(go) {
        AD2CHS0bits.CH0SA = 4;
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        CLEARBIT(AD2CON1bits.DONE);
        ADCValue = ADC2BUF0;
        lcd_locate(0,2);
        lcd_printf_d("JS Min x: %d    ", ADCValue);
        
           
        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;

            if (T > debounceVal) {
                if (pressed == 0) {
                    // pressed
                    min_x = ADCValue;
                    go = 0;
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > debounceVal) {
                pressed = 0;
            }
        } 
    }
    
    init_joystick_axis_y();
    go = 1;
    T = 0;
    pressed = 0;
    released = 0;
    
    while(go) {
        AD2CHS0bits.CH0SA = 5;
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        CLEARBIT(AD2CON1bits.DONE);
        ADCValue = ADC2BUF0;
        lcd_locate(0,3);
        lcd_printf_d("JS Max y: %d    ", ADCValue);
        
           
        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;

            if (T > debounceVal) {
                if (pressed == 0) {
                    // pressed
                    max_y = ADCValue;
                    go = 0;
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > debounceVal) {
                pressed = 0;
            }
        } 
    }
    
    go = 1;
    T = 0;
    pressed = 0;
    released = 0;
    
    while(go) {
        AD2CHS0bits.CH0SA = 5;
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        CLEARBIT(AD2CON1bits.DONE);
        ADCValue = ADC2BUF0;
        lcd_locate(0,4);
        lcd_printf_d("JS Min y: %d    ", ADCValue);
        
           
        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;

            if (T > debounceVal) {
                if (pressed == 0) {
                    // pressed
                    min_y = ADCValue;
                    go = 0;
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > debounceVal) {
                pressed = 0;
            }
        } 
    }
    
    
    int x_pulse = 0;
    int y_pulse = 0;
    
    // initialize x-motor
    int chan = 8;
    motor_init(chan);
    lcd_locate(0,5);
    lcd_printf_d("x-mp: %d    ", x_pulse);
    double servos_low = 900;
    double servos_hi = 2100;
    go = 1;
    T = 0;
    pressed = 0;
    released = 0;
    
    while(go) {
        AD2CHS0bits.CH0SA = 4;
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        CLEARBIT(AD2CON1bits.DONE);
        ADCValue = ADC2BUF0;
        x_pulse = (servos_hi - servos_low)/(max_x - min_x) * (ADCValue - min_x) + servos_low;
        motor_set_duty(chan, x_pulse);
        //lcd_locate(0,7);
        //lcd_printf_d("ret: %d    ", ret);
        lcd_locate(0,5);
        lcd_printf_d("x-mp: %d    ", x_pulse);
           
        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;

            if (T > debounceVal) {
                if (pressed == 0) {
                    // pressed
                    
                    go = 0;
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > debounceVal) {
                pressed = 0;
            }
        } 
    }
    
    // initialize y-motor
    chan = 7;
    motor_init(chan);
    lcd_locate(0,6);
    lcd_printf_d("y-mp: %d    ", y_pulse);
    go = 1;
    T = 0;
    pressed = 0;
    released = 0;
    
    while(go) {
        AD2CHS0bits.CH0SA = 5;
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        CLEARBIT(AD2CON1bits.DONE);
        ADCValue = ADC2BUF0;
        y_pulse = (servos_hi - servos_low)/(max_y - min_y) * (ADCValue - min_y) + servos_low;
        motor_set_duty(chan, y_pulse);
        //lcd_locate(0,7);
        //lcd_printf_d("ret: %d    ", ret);
        lcd_locate(0,6);
        lcd_printf_d("y-mp: %d    ", y_pulse);
           
        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;

            if (T > debounceVal) {
                if (pressed == 0) {
                    // pressed
                    
                    go = 0;
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > debounceVal) {
                pressed = 0;
            }
        } 
    }
            
    while(1);

    
    return 0;
}


