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
#include "lcd.h"
//# define FCY 40000000/16                // instruction cycle clock frequency = 4MHz
# define FCY 800000

inline void uart2_init(uint16_t baud)
{
	/* Stop UART port */
    U2MODEbits.UARTEN = 0; //Disable UART for configuration
    /* Disable Interrupts */
    IEC1bits.U2RXIE = 0;      // Receiver Interrupt Enable bit
    IEC1bits.U2TXIE = 0;       // Transmitter Interrupt Enable bit
    
    /* Clear Interrupt flag bits */
    IFS1bits.U2RXIF = 0;        // Receiver Interrupt Flag Status bit
    IFS1bits.U2TXIF = 0;        // Transmitter Interrupt Flag Status bit
    
    /* Set IO pins */
    TRISFbits.TRISF4 = 1;         //set as input UART2 RX pin  
    TRISFbits.TRISF5 = 0;       //set as output UART2 TX pin
    
    /* baud rate */
    // use the following equation to compute the proper
    // setting for a specific baud rate
    U2MODEbits.ABAUD = 0;         // auto-baud off
    U2MODEbits.BRGH = 0;          // Set low speed baud rate 
    U2BRG = (uint32_t)(FCY)/baud -1; //Set the baudrate to be at 9600
    
    
    /* Operation settings and start port */
    U2MODEbits.STSEL = 0;         // 1 stop bit
    U2MODEbits.PDSEL = 0;         // no parity
    
    
    U2MODEbits.RTSMD = 0; // select simplex mode (no need to control receive time)
    U2MODEbits.UEN = 0;   // select simplex mode 
    U2MODE |= 0x00;
    
    U2MODEbits.UARTEN = 1; //enable UART
    U2STA = 0;       // disable interrupt after Tx and Rx
    U2STAbits.UTXEN = 1; //enable UART TX
    
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
    if (U2STAbits.OERR) {               // Check if there is data overflow error (by checking UxSTAbits.OERR). 
          U2STAbits.OERR = 0;           // You must clear this bit if it has been set in order to receive new data
          return -1;
    }
    
    if (U2STAbits.URXDA) {              // Check if there is data in the buffer (by checking UxSTAbits.URXDA. If yes, then read the data from UxRXREG.
        *data = U2RXREG & 0x00FF;
        return 0;
    } else {
        return 1;
    }
   
    
    
}