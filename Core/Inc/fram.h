/*
 * fram.h
 *
 *  Created on: Mar 10, 2023
 *      Author: Eric
 */

#ifndef INC_FRAM_H_
#define INC_FRAM_H_

#define FRAM_I2C_ADDR 0xA0

void write_byte_to_fram(uint16_t write_address, uint8_t value);
uint8_t read_byte_from_fram(uint16_t read_address);
uint8_t fram_read_dsp_mode();
void fram_write_dsp_mode(uint8_t dsp_mode);

#define FRAM_DSP_MODE_ADDR 	1000


enum {
	FRAM_READ_ERROR
};

#endif /* INC_FRAM_H_ */
