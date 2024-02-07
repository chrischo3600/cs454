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

uint32_t main_counter = 0;
uint16_t s_counter = 0;
uint8_t five_ms_counter = 0;
uint32_t ms_counter = 0;


// T1Interrupt ISR
void __attribute__((__interrupt__)) _T1Interrupt(void) {
    CLEARBIT(IFS0bits.T1IF);
    s_counter++; // Increment a global counter
    TOGGLELED(LED2_PORT);
    TMR1=0;
}

void __attribute__((__interrupt__)) _T2Interrupt(void) {
    CLEARBIT(IFS0bits.T2IF);
    five_ms_counter++; // Increment a global counter
    ms_counter++;
    if (five_ms_counter == 4) {
        TMR2=0;
        TOGGLELED(LED1_PORT);
        five_ms_counter = 0;
    }
}

void __attribute__((__interrupt__)) _INT1Interrupt(void) {
    CLEARBIT(IFS1bits.INT1IF );
    s_counter = 0;
    ms_counter = 0;

}

int main(){
	/* LCD Initialization Sequence */
	__C30_UART=1;	
	led_initialize();
    lcd_initialize();
    lcd_clear();
    
    AD1PCFGHbits.PCFG20 = 1;
    TRISEbits.TRISE8 = 1; // setting it as input
        
    // setup Joystick Button 1
    SETBIT(IEC1bits.INT1IE); //Interrupt Enable Control Register 1
    //External Interrupt 1 Enable bit
    SETBIT(IPC5bits.INT1IP); //Interrupt Priority Control Register 5
    //External Interrupt 1 Priority bits
    CLEARBIT(IFS1bits.INT1IF); //Interrupt Flag Status Register 1
    
    
    //setup Timer 2
    __builtin_write_OSCCONL(OSCCONL | 2);
    CLEARBIT(T2CONbits.TON); // Disable Timer
    CLEARBIT(T2CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE); // Disable Gated Timer mode
    TMR2 = 0x00; // Clear timer register
    T2CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    PR2 = 50; // Load the period value
    IPC1bits.T2IP = 0x01; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T2IF);
    SETBIT(IEC0bits.T2IE); // Enable Timer1 interrupt
    SETBIT(T2CONbits.TON); // Start Timer

    // setup Timer 1
    CLEARBIT(T1CONbits.TON); // Disable Timer
    SETBIT(T1CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T1CONbits.TGATE); // Disable Gated Timer mode
    TMR1 = 0x00; // Clear timer register
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    CLEARBIT(T1CONbits.TSYNC);
    PR1 = 32767; // Load the period value
    IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T1IF);
    SETBIT(IEC0bits.T1IE); // Enable Timer1 interrupt
    SETBIT(T1CONbits.TON); // Start Timer
    
    // setup Timer 3
    CLEARBIT(T3CONbits.TON);
    CLEARBIT(T3CONbits.TCS);
    CLEARBIT(T3CONbits.TGATE);
    TMR3 = 0x00;
    T3CONbits.TCKPS = 0b00;
    CLEARBIT(IFS0bits.T3IF);
    CLEARBIT(IEC0bits.T3IE);
    SETBIT(T3CONbits.TON);
    
    while (1) {
        TMR3 = 0;
        TOGGLELED(LED4_PORT);
        main_counter++;
        if (main_counter >= 25000) {
            lcd_locate(0,0);
            uint16_t min = s_counter / 60;
            uint16_t seconds = (s_counter % 60);
            uint16_t mil_seconds = ms_counter % 1000;
            lcd_printf("%02d:%02d.%03d", min, seconds, mil_seconds);
            main_counter = 0;
            lcd_locate(0,1);
            lcd_printf("cycles: %d", TMR3);
            lcd_locate(0,2);
            lcd_printf("ms: %.4f", (double) TMR3 / 12800);
        }   
    }
    
    return 0;
}

