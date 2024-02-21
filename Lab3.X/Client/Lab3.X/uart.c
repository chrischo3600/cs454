/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 UART2 init,TX,RX functions   */
/*                                                  */
/****************************************************/

#include "uart.h"
#define FCY 40000000                // instruction cycle clock frequency = 4MHz

inline void uart2_init(uint16_t baud)
{
	/* Stop UART port */
    CLEARBIT(U2MODEbits.UARTEN); //Disable UART for configuration
    /* Disable Interrupts */
    CLEARBIT(IEC1bits.U2RXIE);      // Receiver Interrupt Enable bit
    CLEARBIT(IEC1bits.U2TXIE);       // Transmitter Interrupt Enable bit
    
    /* Clear Interrupt flag bits */
    CLEARBIT(IFS1bits.U2RXIF);        // Receiver Interrupt Flag Status bit
    CLEARBIT(IFS1bits.U2TXIF);        // Transmitter Interrupt Flag Status bit
    
    /* Set IO pins (did not change)*/
    SETBIT(TRISFbits.TRISF2);         //set as input UART1 RX pin  
    CLEARBIT(TRISFbits.TRISF3);       //set as output UART1 TX pin
    
    /* baud rate */
    // use the following equation to compute the proper
    // setting for a specific baud rate
    U2MODEbits.BRGH = 0; // Set low speed baud rate (MIGHT BE AN ISSUE)
    U2BRG = (uint32_t)((FCY/baud)/16) -1; //Set the baudrate to be at 9600
    
    
    /* Operation settings and start port */
    CLEARBIT(U2MODEbits.STSEL);         // 1 stop bit
    CLEARBIT(U2MODEbits.PDSEL);         // no parity
    CLEARBIT(U2MODEbits.ABAUD);         // auto-baud off
    
    CLEARBIT(U2MODEbits.RTSMD); // select simplex mode (no need to control receive time)
    CLEARBIT(U2MODEbits.UEN);   // select simplex mode 
    U2MODE |= 0x00;
    
    SETBIT(U2MODEbits.UARTEN); //enable UART
    CLEARBIT(U2STA);       // disable interrupt after Tx and Rx
    SETBIT(U2STAbits.UTXEN); //enable UART TX
    
}

void uart2_send_8(int8_t data)
{
	/* Implement me please. */
    while (U2STAbits.UTXBF);    // Wait on UT2BF bit in the UxSTA register.
    U2TXREG = data;             // Load the UxTXREG register with an 8 bit value.
    while(!U2STAbits.TRMT);     // Wait on TRMT bit in the UxSTA register, 1 = transmission is completed
    
}

int8_t uart2_recv(uint8_t* data)
{
    int8_t ret;
    
	/* Implement me please. */
    if (U2STAbits.OERR) {           // Check if there is data overflow error (by checking UxSTAbits.OERR). You must clear this bit if it has been set in order to receive new data
        U2STAbits.OERR = 0;
    } 
    
    if (U2STAbits.URXDA) {              // Check if there is data in the buffer (by checking UxSTAbits.URXDA. If yes, then read the data from UxRXREG.
        *data = U2RXREG & 0x00FF;
    }
    
    if (U2STAbits.OERR) {               // overflow
        ret = -1;
    } else {
        ret = 0;
    }
        
    return ret;
    
    
    
}
