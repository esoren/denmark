/*
 * infrared.h
 *
 *  Created on: Mar 10, 2023
 *      Author: Eric
 */

#ifndef INC_INFRARED_H_
#define INC_INFRARED_H_

void setup_ir_transmitter();
void ir_send_message(uint8_t addr, uint8_t cmd);
void ir_start_stop_pulse();
void ir_send_one();
void ir_send_zero();
void ir_send_byte(uint8_t payload);
void ir_turn_off_carrier();
void ir_turn_on_carrier();
uint32_t DWT_Delay_Init(void);
int train_minidsp();

//#define IR_TRAINING //turn this flag on to enable compilation of a test routine to train the MiniDSP IR remote commands

enum {
	IR_MASTER_MUTE,
	IR_VOLUME_UP,
	IR_VOLUME_DOWN,
	IR_ANALOG,
	IR_TOSLINK,
	IR_USB,
	IR_CONFIG_1,
	IR_CONFIG_2,
	IR_CONFIG_3,
	IR_CONFIG_4
};

#endif /* INC_INFRARED_H_ */
