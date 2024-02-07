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


int main(){
	/* LCD Initialization Sequence */
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
	lcd_printf("Simon");
    lcd_locate(0,1);
    lcd_printf("Chris");
    lcd_locate(0,2);
    lcd_printf("Kathlyn");
	
    led_initialize();
    int i;
    for(i = 0; i < 3; i++) {
        SETLED(LED4_PORT);
        __delay_ms(100);
        CLEARLED(LED4_PORT);
        __delay_ms(1000);
    }
    
    AD1PCFGHbits.PCFG20 = 1;
    TRISEbits.TRISE8 = 1; // setting it as input
    
    INTCON2bits.INT1EP = 1;
    TRISDbits.TRISD10 = 1;
    
    uint8_t counter = 0;
    
    
    lcd_locate(0,3);
    lcd_printf("%d", counter);
    lcd_locate(0,4);
    lcd_printf("%x", counter);
    
    uint16_t T = 0;
    uint8_t pressed = 0;
    uint16_t released = 0;
	/* Do nothing */
	while(1){

        if (BTN1_PRESSED()) {
            if (released != 0) {
                released = 0;
            }
            T++;
            SETLED(LED1_PORT);

            if (T > 1500) {
                if (pressed == 0) {
                    counter++;
                    
                    lcd_locate(0,3);
                    lcd_printf("%d", counter);
                    lcd_locate(0,4);
                    lcd_printf("%x", counter);
                }
                pressed = 1;
            }
        }
           
        else if (BTN1_RELEASED()) {
            if (T != 0) {
                T = 0;
            }
            released += 1;
            if (released > 1500) {
                pressed = 0;
            }
            CLEARLED(LED1_PORT);
        } 
        
        if (BTN2_PRESSED()) {
            SETLED(LED2_PORT);
        } else if (BTN2_RELEASED()) {
            CLEARLED(LED2_PORT);
        } 
        
        if (BTN1_PRESSED() ^ BTN2_PRESSED()) {
            SETLED(LED3_PORT);
        } else {
            CLEARLED(LED3_PORT);
        }
        
	}
    
    return 0;
}


