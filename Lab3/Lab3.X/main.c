//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include <p33Fxxxx.h>
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



int main(void)
{	
	/* Q: What is my purpose? */
	/* A: You pass butter. */
	/* Q: Oh. My. God. */
    
    lcd_initialize();
    lcd_clear();
    lcd_locate(0,0);
    uart2_init(9600);
    
    int fail = 0;
    lcd_locate(0,0);
    lcd_printf_d("Recv fail: %d times", fail);
    while(1);
    
    while(1) {
       // Acknowledge messages
       
        // Read the start bit
                   // Wait for start bit
        uint8_t* start_byte;
        while (uart2_recv(start_byte) != 0);         // Receive start bit
        lcd_locate(0,5);
        lcd_printf_d("start_byte: %x", *start_byte);
        while(1);
//        if (*start_byte != 0) {
//            fail++;
//            int a = 0;
//            while (U2STAbits.URXDA) {
//                a = U2RXREG & 0x00FF;
//            }
//            uart2_send_8(MSG_NACK);
//            continue;
//        }
        lcd_locate(0,2);
        lcd_printf_d("hi");
        while(1);
        uint8_t* crc_byte1;
        *crc_byte1 = 0;
        while (uart2_recv(crc_byte1) != 0);
        lcd_locate(0,3);
        lcd_printf_d("%x", *crc_byte1);
        uint8_t* crc_byte2;
        while (uart2_recv(crc_byte2) != 0);
        lcd_locate(0,4);
        lcd_printf_d("%x", *crc_byte2);
        while(1);
    }
}	

