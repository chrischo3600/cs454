//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <stdlib.h> 
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
    
    int a = 0;
    while (U2STAbits.URXDA) {
        a = U2RXREG & 0x00FF;
    }
    while(1) {
       //Acknowledge messages
        // Read the start bit
        // Wait for start bit
        
        lcd_locate(0,0);
        lcd_printf_d("Recv fail: %d times", fail);
        uint8_t buffer[259] = {0};
        buffer[0] = -1;
        buffer[1] = -1;
        buffer[2] = -1;
        uint8_t i = 0;
        
        for (i = 0; i < 4; i++) {
            while (uart2_recv(&buffer[i]) != 0);
        }
        lcd_clear();
        for (i = 4; i < 4 + buffer[3]; i++) {
            while (uart2_recv(&buffer[i]) != 0);
        }
        
        //while (uart2_recv(buffer[i]) == 0) {
        //    i++;
        //};
        

        uint16_t crc_rec = 0;
        crc_rec = (buffer[1] << 8) | buffer[2];
        
        uint8_t N = 0;
        N = buffer[3];

        char message[N]; 
        uint16_t crc = 0;
        int j;
        // reading message
        for (j = 4; j < (N + 4); j++) {
            message[j - 4] = (char) buffer[j];
            crc = crc_update(crc, buffer[j]);
        }
        message[j - 4] = '\0';
 
        lcd_locate(0,6);
        lcd_printf_d("start_byte: %x, %x, %x", buffer[0], buffer[1], buffer[2]);
        lcd_locate(0,1);
        lcd_printf_d("crc_rec: %x", crc_rec);
        lcd_locate(0,2);
        lcd_printf_d("crc: %x", crc);
        lcd_locate(0,3);
        lcd_printf_d("length: %d", N);
        lcd_locate(0,4);
        lcd_printf_d(message);
        lcd_locate(0,5);
        lcd_printf_d("i: %d", i);
        
        if (crc_rec == crc) {
            uart2_send_8(MSG_ACK);
            fail = 0;
            lcd_locate(0,0);
            lcd_printf("Recv fail: %d times", fail);
        } else {
            fail++;
            lcd_locate(0,0);
            lcd_printf("Recv fail: %d times", fail);
            int a = 0;
            while (U2STAbits.URXDA) {
                a = U2RXREG & 0x00FF;
            }
            uart2_send_8(MSG_NACK);
        }
    }
}	

