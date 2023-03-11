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

QueueHandle_t xPowerQueue;


void StartPowerTask(void const *argument) {



	uint8_t power_state = POWER_STATE_PUP_SEQUENCE;
	uint8_t standby_toggle = 0;

	uint8_t powerMessage;
	BaseType_t xStatus;

	for (;;)
	{

		standby_toggle = 0;


		if (uxQueueMessagesWaiting(xPowerQueue) > 0) {
			xStatus = xQueueReceive(xPowerQueue, &powerMessage, 0);
			if (xStatus == pdPASS) {
				if(powerMessage == POWER_TOGGLE_STANDBY) {
					standby_toggle = 1;
				}
			}
		}


		switch(power_state) {

		case POWER_STATE_PUP_SEQUENCE:
			pup_sequence();

			power_state = enter_power_state_standby();
			break;

		case POWER_STATE_STANDBY:

			if(standby_toggle) {
				power_state = enter_power_state_amps_on();
			}

			break;


		case POWER_STATE_AMPS_ON:

			if(standby_toggle) {
				power_state = enter_power_state_standby();
			}
			break;
		}

		osDelay(100);
	}

}

uint8_t enter_power_state_standby(void) {

	displayMessage_t displayMessage;
	uint8_t temperatureMessage;
	uint8_t fanMessage;

	displayMessage.displayCommand = SET_LED_STATE;
	displayMessage.modify_mask = LED_STANDBY_ON;
	displayMessage.new_values = 0xffff;
	xQueueSend(xDisplayQueue, &displayMessage, 0);

	osDelay(100);

	set_clear_amp_mute(HF_AMP, MUTE_ON);
	set_clear_amp_mute(MF_AMP, MUTE_ON);

	osDelay(100);

	set_clear_amp_standby(HF_AMP, STANDBY_ON);
	set_clear_amp_standby(MF_AMP, STANDBY_ON);
	set_clear_amp_standby(LF_AMP, STANDBY_ON);

	temperatureMessage = TEMPERATURE_MONITOR_STOP;
	xQueueSend(xTemperatureQueue, &temperatureMessage, 0);

	fanMessage = FAN_MONITOR_STOP;
	xQueueSend(xFanQueue, &fanMessage, 0);

	fanMessage = FAN_SET_SPEED_OFF;
	xQueueSend(xFanQueue, &fanMessage, 0);

	return POWER_STATE_STANDBY;
}

uint8_t enter_power_state_amps_on(void) {

	displayMessage_t displayMessage;
	uint8_t temperatureMessage;
	uint8_t fanMessage;

	displayMessage.displayCommand = SET_LED_STATE;
	displayMessage.modify_mask = LED_STANDBY_ON;
	displayMessage.new_values = 0;
	xQueueSend(xDisplayQueue, &displayMessage, 0);
	osDelay(100);

	set_clear_amp_standby(LF_AMP, STANDBY_OFF); //power up LF and power supply first

	osDelay(500);

	set_clear_amp_standby(HF_AMP, STANDBY_OFF);
	set_clear_amp_standby(MF_AMP, STANDBY_OFF);


	osDelay(400);

	set_clear_amp_mute(HF_AMP, MUTE_OFF);
	set_clear_amp_mute(MF_AMP, MUTE_OFF);

	temperatureMessage = TEMPERATURE_MONITOR_START;
	xQueueSend(xTemperatureQueue, &temperatureMessage, 0);

	fanMessage = FAN_SET_SPEED_NOMINAL;
	xQueueSend(xFanQueue, &fanMessage, 0);

	osDelay(1000); //wait for fan to spin up

	fanMessage = FAN_MONITOR_START;
	xQueueSend(xFanQueue, &fanMessage, 0);

	return POWER_STATE_AMPS_ON;
}


void pup_sequence() {
	uint8_t dsp_mode = DSP_MODE_1;

	displayMessage_t displayMessage;

	//turn on MONITOR 1 LED
	displayMessage.displayCommand = SET_LED_STATE;
	displayMessage.modify_mask = LED_MONITOR_OVERTEMP;
	displayMessage.new_values = 0xffff;
	xQueueSend(xDisplayQueue, &displayMessage, 0);
	osDelay(500);

	dsp_mode = fram_read_dsp_mode();
	// load source select from FRAM
	displayMessage.displayCommand = SET_LED_STATE;
	displayMessage.modify_mask = LED_MONITOR_CLIP;
	displayMessage.new_values = 0xffff;
	xQueueSend(xDisplayQueue, &displayMessage, 0);

	osDelay(500); //wait for DSP to finish bootup

	set_dsp_mode(dsp_mode); //write DSP state
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

	return;
}

void set_clear_amp_standby(uint8_t amplifier, uint8_t standby_state) {
	switch(amplifier) {

	case LF_AMP:
		if(standby_state == STANDBY_ON) {
			HAL_GPIO_WritePin(STANDBY_LF_AMP_OUTPUT_GPIO_Port, STANDBY_LF_AMP_OUTPUT_Pin, GPIO_PIN_SET);
		} else if (standby_state == STANDBY_OFF) {
			HAL_GPIO_WritePin(STANDBY_LF_AMP_OUTPUT_GPIO_Port, STANDBY_LF_AMP_OUTPUT_Pin, GPIO_PIN_RESET);
		} else {
			//should not reach here, do nothing.
			return;
		}
		break;

	case MF_AMP:
		if(standby_state == STANDBY_ON) {
			HAL_GPIO_WritePin(STANDBY_MF_AMP_GPIO_Port, STANDBY_MF_AMP_Pin, GPIO_PIN_SET);
		} else if (standby_state == STANDBY_OFF) {
			HAL_GPIO_WritePin(STANDBY_MF_AMP_GPIO_Port, STANDBY_MF_AMP_Pin, GPIO_PIN_RESET);
		} else {
			//should not reach here, do nothing.
			return;
		}
		break;

	case HF_AMP:
		if(standby_state == STANDBY_ON) {
			HAL_GPIO_WritePin(STANDBY_HF_AMP_GPIO_Port, STANDBY_HF_AMP_Pin, GPIO_PIN_SET);
		} else if (standby_state == STANDBY_OFF) {
			HAL_GPIO_WritePin(STANDBY_HF_AMP_GPIO_Port, STANDBY_HF_AMP_Pin, GPIO_PIN_RESET);
		} else {
			//should not reach here, do nothing.
			return;
		}
		break;

	}

	return;

}

void set_clear_amp_mute(uint8_t amplifier, uint8_t mute_state) {
	switch(amplifier) {

	case LF_AMP:
		//LF amp does not have a mute function
		return;
		break;

	case MF_AMP:
		if(mute_state == MUTE_ON) {
			HAL_GPIO_WritePin(MUTE_MF_AMP_GPIO_Port, MUTE_MF_AMP_Pin, GPIO_PIN_SET);
		} else if (mute_state == MUTE_OFF) {
			HAL_GPIO_WritePin(MUTE_MF_AMP_GPIO_Port, MUTE_MF_AMP_Pin, GPIO_PIN_RESET);
		} else {
			//should not reach here, do nothing.
			return;
		}
		break;

	case HF_AMP:
		if(mute_state == MUTE_ON) {
			HAL_GPIO_WritePin(MUTE_HF_AMP_GPIO_Port, MUTE_HF_AMP_Pin, GPIO_PIN_SET);
		} else if (mute_state == MUTE_OFF) {
			HAL_GPIO_WritePin(MUTE_HF_AMP_GPIO_Port, MUTE_HF_AMP_Pin, GPIO_PIN_RESET);
		} else {
			//should not reach here, do nothing.
			return;
		}
		break;

	}

	return;

}





