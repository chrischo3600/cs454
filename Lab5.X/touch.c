/*
 * File:   touch.c
 * Author: team-7a
 *
 * Created on April 1, 2024, 12:21 PM
 */


#include "xc.h"
#include "types.h"

//Perform any necessary initialization for the touchscreen
//and associated ADC pins.
void touch_init(void) {
    //set up the I/O pins E1, E2, E3 to be output pins
    CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output
    //set up the I/O pins E1, E2, E3 so that the touchscreen X-coordinate pin
    //connects to the ADC
    CLEARBIT(LATEbits.LATE1);
    SETBIT(LATEbits.LATE2);
    SETBIT(LATEbits.LATE3);

}

//to sample on either the x- or y-dimension.
void touch_select_dim(uint8_t dimension) {


}

// Acquire a position sample from the touchscreen on
//the dimension selected with the previous touch select dim(...) command
uint16_t touch_read(void) {
    
}