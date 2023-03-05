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
#include "state.h"

void StartInputTask(void const *argument) {

	for (;;)
	{

		if(HAL_GPIO_ReadPin(BUTTON1_INPUT_GPIO_Port, BUTTON1_INPUT_Pin) == GPIO_PIN_RESET) {
			update_mask();
		}

		osDelay(250);

	}





}
