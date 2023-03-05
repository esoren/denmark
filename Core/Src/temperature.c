/*
 * temperature.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "gpio.h"

void StartTemperatureTask(void const *argument) {

	for(;;)
	{
		osDelay(1000);
	}

}
