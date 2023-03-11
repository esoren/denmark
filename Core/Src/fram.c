/*
 * fram.c
 *
 *  Created on: Mar 10, 2023
 *      Author: Eric
 */


#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "fram.h"
#include "i2c.h"




void write_byte_to_fram(uint16_t write_address, uint8_t value) {
	HAL_StatusTypeDef status;

	uint8_t buffer[2];
	buffer[0] = value;

	status = HAL_I2C_Mem_Write(&hi2c1, FRAM_I2C_ADDR, write_address, I2C_MEMADD_SIZE_16BIT, buffer, 1, HAL_MAX_DELAY);

	if ( status != HAL_OK ) {
		return; //todo: this should have error handling

	}
}

uint8_t read_byte_from_fram(uint16_t read_address) {
	HAL_StatusTypeDef status;

	uint8_t buffer[1];

	HAL_I2C_Mem_Read(&hi2c1, FRAM_I2C_ADDR, read_address, I2C_MEMADD_SIZE_16BIT, buffer, 1, HAL_MAX_DELAY);

	if ( status != HAL_OK ) {
		return FRAM_READ_ERROR; //todo: this should have error handling

	} else {
		return buffer[0];
	}
}




