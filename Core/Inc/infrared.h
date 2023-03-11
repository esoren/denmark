/*
 * infrared.h
 *
 *  Created on: Mar 10, 2023
 *      Author: Eric
 */

#ifndef INC_INFRARED_H_
#define INC_INFRARED_H_

void setup_ir_transmitter();
void ir_send_message(uint16_t payload);
void ir_start_stop_pulse();
void ir_send_one();
void ir_send_zero();
uint32_t DWT_Delay_Init(void);


#endif /* INC_INFRARED_H_ */
