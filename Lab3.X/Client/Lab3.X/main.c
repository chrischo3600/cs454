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
    uint8_t* data;
    int res = -1;
    int fail = 0;
    uint16_t crc_rec;
    lcd_locate(0,0);
    lcd_printf("Recv fail: %d times", fail);
    
    while(1) {
       // Acknowledge messages
       
        // Read the start bit
        while (!U2STAbits.URXDA); // Wait for start bit
        uart2_recv(data);         // Receive start bit
        if (data != 0) {
            uart2_send_8(MSG_NACK);
            fail++;
            lcd_locate(0,0);
            lcd_printf("Recv fail: %d times", fail);
        }

        crc_rec = 0;
        // Read crc
        int j;
        for (j = 0; j < 2; j++) {
            uart2_recv(data);
            crc_rec = (crc_rec << 8) | *data;
        }
        
        lcd_locate(0, 1);
        lcd_printf("CRC: %x", crc_rec);
                
        // read N
        uart2_recv(data);
        int N = *data;
        char* message[N]; 
        uint16_t crc = 0xffff;
        
        int i;
        // reading message
        for (i = 0; i < N; i++) {
            uart2_recv(data);
            message[i] = (char)*data;
            crc = crc_update(crc, *data);
        }
        message[i] = '/0';
        
        // Display message 
        lcd_printf(message);
        
        // crc update makes sure you match the message the server sends
        // if it matches, send an ack, else nack
        if (crc_rec == crc) {
            uart2_send_8(MSG_ACK);
            fail = 0;
            lcd_locate(0,0);
            lcd_printf("Recv fail: %d times", fail);
        } else {
            uart2_send_8(MSG_NACK);
            fail++;
            lcd_locate(0,0);
            lcd_printf("Recv fail: %d times", fail);
        }
        
    }
}	

