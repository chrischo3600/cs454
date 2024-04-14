/*
 * File:   touch.c
 * Author: team-7a
 *
 * Created on April 7, 2024, 12:09 PM
 */


#include "xc.h"
#include "types.h"


//Perform any necessary initialization for the touchscreen
//and associated ADC pins.
void touch_init(char dim) {
    if (dim == 'x') {
        CLEARBIT(AD1CON1bits.ADON);
        
        //set up the I/O pins E1, E2, E3 to be output pins
        CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
        CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
        CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output
    
        //set up the I/O pins E1, E2, E3 so that the touchscreen X-coordinate pin
        //connects to the ADC
        SETBIT(LATEbits.LATE1);
        CLEARBIT(LATEbits.LATE2);
        CLEARBIT(LATEbits.LATE3);
        
        CLEARBIT(AD1PCFGLbits.PCFG15);

        // use 12-bit 1 channel
        SETBIT(AD1CON1bits.AD12B);
        // integer
        CLEARBIT(AD1CON1bits.FORM0);
        CLEARBIT(AD1CON1bits.FORM1);
        // automatic conversion
        SETBIT(AD1CON1bits.SSRC0);
        SETBIT(AD1CON1bits.SSRC1);
        SETBIT(AD1CON1bits.SSRC2);

        // set scanning sample to 0
        AD1CON2 = 0;

        // Conversion clock source
        CLEARBIT(AD1CON3bits.ADRC); // internal clock source
        AD1CON3bits.SAMC = 0x1F;    // sample-to-conversion clock = 31Tad
        AD1CON3bits.ADCS = 0x2;     // Tad = 3Tcy (Time cycles)
        
        
    } else if (dim == 'y'){
        CLEARBIT(AD1CON1bits.ADON);
    
        //set up the I/O pins E1, E2, E3 to be output pins
        CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
        CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
        CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output
        
        //set up the I/O pins E1, E2, E3 so that the touchscreen y-coordinate pin
        //connects to the ADC
        CLEARBIT(LATEbits.LATE1);
        SETBIT(LATEbits.LATE2);
        SETBIT(LATEbits.LATE3);
        
        CLEARBIT(AD1PCFGLbits.PCFG9);

        // use 12-bit 1 channel
        SETBIT(AD1CON1bits.AD12B);
        // integer
        CLEARBIT(AD1CON1bits.FORM0);
        CLEARBIT(AD1CON1bits.FORM1);
        // automatic conversion
        SETBIT(AD1CON1bits.SSRC0);
        SETBIT(AD1CON1bits.SSRC1);
        SETBIT(AD1CON1bits.SSRC2);

        // set scanning sample to 0
        AD1CON2 = 0;

        // Conversion clock source
        CLEARBIT(AD1CON3bits.ADRC); // internal clock source
        AD1CON3bits.SAMC = 0x1F;    // sample-to-conversion clock = 31Tad
        AD1CON3bits.ADCS = 0x2;     // Tad = 3Tcy (Time cycles)

    }

}

//to sample on either the x- or y-dimension.
void touch_select_dim(char dimension) {
    if (dimension == 'x') {
        AD1CHS0bits.CH0SA = 15;
        AD1CON1bits.ADON = 1;
    } else if (dimension == 'y') {
        AD1CHS0bits.CH0SA = 9;
        AD1CON1bits.ADON = 1;
    }
}

// Acquire a position sample from the touchscreen on
//the dimension selected with the previous touch select dim(...) command
uint16_t touch_read(void) {
    SETBIT(AD1CON1bits.SAMP);
    while(!AD1CON1bits.DONE);
    CLEARBIT(AD1CON1bits.DONE);
    return ADC1BUF0;
}