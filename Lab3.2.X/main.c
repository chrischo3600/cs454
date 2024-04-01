//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <stdlib.h> 
#include <libpic30.h>

#include <p33Fxxxx.h>
#include <p33FJ256MC710.h>
#include "types.h"
#include "uart.h"
#include "crc16.h"
#include "lab3.h"
#include "lcd.h"
#include "timer.h"

// Primary (XT, HS, EC) Oscillator without PLL
_FOSCSEL(FNOSC_PRIPLL);
// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystanl
_FOSC(OSCIOFNC_ON & POSCMD_XT);
// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);
// Disable Code Protection
_FGS(GCP_OFF);   

int timer_fired = 0;

void __attribute__((__interrupt__)) _T1Interrupt(void) {
    TMR1=0;
    timer_fired = 1;
    CLEARBIT(IFS0bits.T1IF);
}

void init_joystick_axis_x() {
    // turn ADC OFF
    
    CLEARBIT(AD2CON1bits.ADON);
    
    // Select Port B Pin RB 4 to Analog
    SETBIT(TRISBbits.TRISB4);
    CLEARBIT(AD2PCFGLbits.PCFG4);
    
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


int main(void)
{	
    __C30_UART=1;
    
    lcd_initialize();
    lcd_clear();
    lcd_locate(0,0);
    uart2_init(9600);
    
    int fail = 0;
    
    int a = 0;
    while (U2STAbits.URXDA) {
        a = U2RXREG & 0x00FF;
    }
    lcd_locate(0,0);
    lcd_printf_d("-- Lab 05 --");
    init_joystick_axis_x();
    
    while(1) {
        AD2CON1bits.SAMP = 1;
        while(!AD2CON1bits.DONE);
        int16_t ADCValue = ADC2BUF0;
        lcd_locate(1,0);
        lcd_printf_d("ADCValue: %d", ADCValue);
    }

    //set_timer1(period); 
    
}	

