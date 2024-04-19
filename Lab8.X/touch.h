/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*                                                  */
/****************************************************/

#ifndef TOUCH_H
#define TOUCH_H
#include "types.h"

//Perform any necessary initialization for the touchscreen
//and associated ADC pins.
void touch_init(void);

//to sample on either the x- or y-dimension.
void touch_select_dim(uint8_t dimension);

// Acquire a position sample from the touchscreen on
//the dimension selected with the previous touch select dim(...) command
uint16_t touch_read(void);


#endif