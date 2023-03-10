/*
 * power.c
 *
 *  Created on: Mar 6, 2023
 *      Author: Eric
 */

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "temperature.h"
#include "display.h"
#include "fan.h"
#include "fault.h"
#include "power.h"
#include "dsp.h"
#include "gpio.h"

extern QueueHandle_t xDisplayQueue;
extern QueueHandle_t xTemperatureQueue;
extern QueueHandle_t xFanQueue;

void StartPowerTask(void const *argument) {


	displayMessage_t displayMessage;
	uint8_t temperatureMessage;
	uint8_t fanMessage;

	BaseType_t xStatus;


	uint8_t power_state = POWER_STATE_PUP_SEQUENCE;

	for (;;)
	{


		switch(power_state) {

		case POWER_STATE_PUP_SEQUENCE:
			//turn on MONITOR 1 LED
			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_OVERTEMP;
			displayMessage.new_values = 0xffff;
			xQueueSend(xDisplayQueue, &displayMessage, 0);

			osDelay(500);

			//load dsp state from FRAM
			// load source select from FRAM
			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_CLIP;
			displayMessage.new_values = 0xffff;
			xQueueSend(xDisplayQueue, &displayMessage, 0);

			osDelay(500); //wait for DSP to finish bootup

			set_dsp_mode(DSP_MODE_1); //write DSP state
			//write source select
			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_PROTECT;
			displayMessage.new_values = 0xffff;
			xQueueSend(xDisplayQueue, &displayMessage, 0);

			osDelay(500); //wait for DSP to stabilize after writing new parameters

			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_FAN;
			displayMessage.new_values = 0xffff;
			xQueueSend(xDisplayQueue, &displayMessage, 0);

			osDelay(500); //wait for DSP to stabilize after writing new parameters

			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_FAN | LED_MONITOR_PROTECT | LED_MONITOR_CLIP | LED_MONITOR_OVERTEMP;
			displayMessage.new_values = 0;
			xQueueSend(xDisplayQueue, &displayMessage, 0);

			power_state = POWER_STATE_STANDBY;
			break;

		case POWER_STATE_STANDBY:




			//placeholder for test:
			temperatureMessage = TEMPERATURE_MONITOR_START;
			xQueueSend(xTemperatureQueue, &temperatureMessage, 0);

			fanMessage = FAN_MONITOR_START;
			xQueueSend(xFanQueue, &fanMessage, 0);


			power_state = POWER_STATE_AMPS_ON;
			break;
		case POWER_STATE_AMPS_ON:
			HAL_GPIO_WritePin(STANDBY_LF_AMP_GPIO_Port, STANDBY_LF_AMP_Pin, GPIO_PIN_RESET);
			break;
		}

		osDelay(100);
	}

}


void set_clear_amp_standby(uint8_t amplifier, uint8_t standby_state) {
	switch(amplifier) {

	/*case LF_AMP:
		if(standby_state == AMP_IN_STANDBY) {
			//HAL_GPIO_WritePin(STANDBY_LF_PIN_GPIO_Port, STANDBY_LF_PIN, GPIO_PIN_RESET)
		}
	*/

	}


}



