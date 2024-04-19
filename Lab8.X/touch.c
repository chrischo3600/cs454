#include "touch.h"
#include "lcd.h"

uint8_t sample_pin = 15;
// RB15 B ADC1CH15 Balance Board?s x-axis
// RB9 B ADC1CH9 Balance Board?s y-axis

void touch_init(void){
    CLEARBIT(AD1CON1bits.ADON);
    SETBIT(TRISBbits.TRISB9);
    SETBIT(TRISBbits.TRISB15);
    CLEARBIT(AD1PCFGLbits.PCFG9);//set AD1 AN9 input pin as analog
    CLEARBIT(AD1PCFGLbits.PCFG15);//set AD1 AN15 input pin as analog
    //Configure AD1CON1
    SETBIT(AD1CON1bits.AD12B); //set 10b Operation Mode
    AD1CON1bits.FORM = 0; //set integer output
    AD1CON1bits.SSRC = 0x7; //set automatic conversion
    //Configure AD1CON2
    AD1CON2 = 0; //not using scanning sampling
    //Configure AD1CON3
    CLEARBIT(AD1CON3bits.ADRC); //internal clock source
    AD1CON3bits.SAMC = 0x1F; //sample-to-conversion clock = 31Tad
    AD1CON3bits.ADCS = 0x2; //Tad = 3Tcy (Time cycles)
    //Leave AD1CON4 at its default value
    //enable ADC
    SETBIT(AD1CON1bits.ADON);

    //set up the I/O pins E1, E2, E3 to be output pins
    CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output

}
void touch_select_dim(uint8_t dimension){ // 0: x-axis, 1: y-axis
    if(dimension == 0) {
        sample_pin = 15;
        CLEARBIT(LATEbits.LATE1);
        SETBIT(LATEbits.LATE2);
        SETBIT(LATEbits.LATE3);
    }
    else if(dimension == 1){
        sample_pin = 9;
        SETBIT(LATEbits.LATE1);
        CLEARBIT(LATEbits.LATE2);
        CLEARBIT(LATEbits.LATE3);
    }

}
uint16_t touch_read(void){
    AD1CHS0bits.CH0SA = sample_pin; //set ADC to Sample AN4 pin
    SETBIT(AD1CON1bits.SAMP); //start to sample
    while(!AD1CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD1CON1bits.DONE); //MUST HAVE! clear conversion done bit
    return ADC1BUF0; //return sample
}