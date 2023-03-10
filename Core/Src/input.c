/*
 * input.c
 *
 *  Created on: Mar 4, 2023
 *      Author: Eric
 */


#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "display.h"
#include "queue.h"
#include "gpio.h"
#include "dsp.h"
#include "fault.h"
#include "temperature.h"
#include "power.h"

extern QueueHandle_t xTemperatureQueue;
extern QueueHandle_t xPowerQueue;

void StartInputTask(void const *argument) {

	uint8_t powerMessage;

	for (;;)
	{
		//todo: add proper debouncing

		if(HAL_GPIO_ReadPin(BUTTON1_INPUT_GPIO_Port, BUTTON1_INPUT_Pin) == GPIO_PIN_RESET) {
			powerMessage = POWER_TOGGLE_STANDBY;
			xQueueSend(xPowerQueue, &powerMessage,0);
			osDelay(500);
		}

		if(HAL_GPIO_ReadPin(BUTTON2_INPUT_GPIO_Port, BUTTON2_INPUT_Pin) == GPIO_PIN_RESET) {
			next_dsp_mode();
			osDelay(500);
		}

		if(HAL_GPIO_ReadPin(BUTTON3_INPUT_GPIO_Port, BUTTON3_INPUT_Pin) == GPIO_PIN_RESET) {
			update_mask_state();
			osDelay(500);
		}

		if(HAL_GPIO_ReadPin(BUTTON4_INPUT_GPIO_Port, BUTTON4_INPUT_Pin) == GPIO_PIN_RESET) {
			set_clear_fault_flags(FAULT_ALL, 0);
			osDelay(500);
		}

		if(HAL_GPIO_ReadPin(BUTTON5_INPUT_GPIO_Port, BUTTON5_INPUT_Pin) == GPIO_PIN_RESET) {
			//todo: placeholder for change input source v
			osDelay(500);
		}


		osDelay(25);

	}





}
