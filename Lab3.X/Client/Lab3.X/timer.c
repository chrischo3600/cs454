/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 timer initialization func.   */
/*                                                  */
/****************************************************/


#include "timer.h"

//period in ms
void set_timer1(uint16_t period){
	/* Implement me please. */
    // period in ms to seconds
    period = (int) (period/32.767); // !!
    
    // setup Timer 1
    CLEARBIT(T1CONbits.TON); // Disable Timer
    SETBIT(T1CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T1CONbits.TGATE); // Disable Gated Timer mode
    TMR1 = 0x00; // Clear timer register
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    CLEARBIT(T1CONbits.TSYNC);
    PR1 = period; // Load the period value
    IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
    CLEARBIT(IFS0bits.T1IF);
    SETBIT(IEC0bits.T1IE); // Enable Timer1 interrupt
    SETBIT(T1CONbits.TON); // Start Timer
    
}

