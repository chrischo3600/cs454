/*
 * File:   motor.c
 * Author: team-7a
 *
 * Created on April 1, 2024, 12:21 PM
 */



#include "xc.h"
#include "types.h"

void motor_init() {
    // disable timer 2
    CLEARBIT(T2CONbits.TON);

    // setup timer 2 for no interrupts, 20 ms period 
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);
    TMR2 = 0x00;
    T2CONbits.TCKPS= 0b10;  // 1:64
    CLEARBIT(IEC0bits.T2IE);
    CLEARBIT(IFS0bits.T2IF);
    PR2 = 4000;

}

void motor_switch(uint8_t chan) {
    
    // setup OC 8 for x-axis motor
    if (chan == 8) {
        
        CLEARBIT(T2CONbits.TON);
        TMR2 = 0x00;
        // configure OC 8 (x-axis)
        CLEARBIT(TRISDbits.TRISD7); // set OC8 = RD7 as output
        OC8CONbits.OCM = 0b110;     // set PWM, no fault mode 

        // enable timer2
        SETBIT(T2CONbits.TON);
    
    } else if (chan == 7) {
        // disable timer 2
        CLEARBIT(T2CONbits.TON);
        TMR2 = 0x00;
        // configure OC 7 (y-axis)
        CLEARBIT(TRISDbits.TRISD7); // set OC7 = RD7 as output !! not yet
        OC7CONbits.OCM = 0b110;     // set PWM, no fault mode 

        // enable timer2
        SETBIT(T2CONbits.TON);
    
    } 

}

void motor_set_duty(uint8_t chan, uint16_t duty_us) {
    // 4000 / 20 ms
    uint16_t duty_cycle = 4000 -(int) (duty_us/1000.0 * (4000/20));
    
    if (chan == 8) {                
        OC8RS = duty_cycle;               
        
    } else if (chan == 7) {          
        OC7RS = duty_cycle;               
    
    }
    

}
