/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*                                                  */
/****************************************************/

void motor_init();

void motor_switch(uint8_t chan);

void motor_set_duty(uint8_t chan, uint16_t duty us);