/*
 * display.c
 *
 *  Created on: Mar 2, 2023
 *      Author: Eric
 */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "display.h"
#include "queue.h"
#include "gpio.h"

QueueHandle_t xDisplayQueue;

void StartDisplayTask(void const *argument)
{

	displayMessage_t displayMessage;
	BaseType_t xStatus;

	uint8_t led_state[16]; 	//the current state of each LED. 0 = off, 1 = on
	uint8_t led_mask[16];  	//the mask of each LED. 0 = off, 1 = on.
							//if the mask is set to zero, the LED will not display even if the state is 1.


	for (int i = 0; i < NUM_LEDS; i++) //initialize all masks and states to zero
	{
		led_state[i] = 0;
		led_mask[i] = 0;
	}

	initialize_leds();

	led_state[LED_POWER_ON] = 1; //turn on the power LED by default
	led_mask[LED_POWER_ON] = 1;


	for (;;)
	{

		if (uxQueueMessagesWaiting(xDisplayQueue) > 0) {
			xStatus = xQueueReceive(xDisplayQueue, &displayMessage, 0);

			if (xStatus == pdPASS) {

				switch (displayMessage.displayCommand) {
				case SET_STATE:
					break;
				case SET_MASK:
					break;
				case SET_FADE:
					break;
				case BLINK_MASK:
					break;
				}
			}
		}



		set_led_states(led_state, led_mask);
		osDelay(100);

	}
}


void initialize_leds()
{
	//assumes that all LED pins are setup as outputs using STM32Cube-generated gpio.c
	//todo: configure PWM for user led brightness level (fade)
	return;
}




//this function handles the mapping of the state and mask arrays to the physical LED pins.
//is is assumed that each pin is set as an output.
//if the mask and state array are each set to 1 for a given pin, then the pin will be set
//if either the mask or the state array is set to 0, the pin will be cleared.
//this is meant to be called each time a change is made to the state or mask arrays

//todo: consider using a macro define to make this more readable


void set_led_states(uint8_t led_state[], uint8_t led_mask[])
{
	if (led_state[LED_POWER_ON] && led_mask[LED_POWER_ON] ) {
		HAL_GPIO_WritePin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_STANDBY_ON] && led_mask[LED_STANDBY_ON] ) {
		HAL_GPIO_WritePin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_DSP_1] && led_mask[LED_DSP_1] ) {
		HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_DSP_2] && led_mask[LED_DSP_2] ) {
		HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_DSP_3] && led_mask[LED_DSP_3] ) {
		HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_DSP_4] && led_mask[LED_DSP_4] ) {
		HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_MONITOR_OVERTEMP] && led_mask[LED_MONITOR_OVERTEMP] ) {
		HAL_GPIO_WritePin(LED_MONITOR_1_GPIO_Port, LED_MONITOR_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_1_GPIO_Port, LED_MONITOR_1_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_MONITOR_CLIP] && led_mask[LED_MONITOR_CLIP] ) {
		HAL_GPIO_WritePin(LED_MONITOR_2_GPIO_Port, LED_MONITOR_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_2_GPIO_Port, LED_MONITOR_2_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_MONITOR_PROTECT] && led_mask[LED_MONITOR_PROTECT] ) {
		HAL_GPIO_WritePin(LED_MONITOR_3_GPIO_Port, LED_MONITOR_3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_3_GPIO_Port, LED_MONITOR_3_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_MONITOR_FAN] && led_mask[LED_MONITOR_FAN] ) {
		HAL_GPIO_WritePin(LED_MONITOR_4_GPIO_Port, LED_MONITOR_4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_4_GPIO_Port, LED_MONITOR_4_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_FAULT_HF_TEMP] && led_mask[LED_FAULT_HF_TEMP] ) {
		HAL_GPIO_WritePin(LED_FAULT_1_GPIO_Port, LED_FAULT_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_1_GPIO_Port, LED_FAULT_1_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_FAULT_MF_TEMP] && led_mask[LED_FAULT_MF_TEMP] ) {
		HAL_GPIO_WritePin(LED_FAULT_2_GPIO_Port, LED_FAULT_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_2_GPIO_Port, LED_FAULT_2_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_FAULT_LF_TEMP] && led_mask[LED_FAULT_LF_TEMP] ) {
		HAL_GPIO_WritePin(LED_FAULT_3_GPIO_Port, LED_FAULT_3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_3_GPIO_Port, LED_FAULT_3_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_FAULT_CLIP] && led_mask[LED_FAULT_CLIP] ) {
		HAL_GPIO_WritePin(LED_FAULT_4_GPIO_Port, LED_FAULT_4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_4_GPIO_Port, LED_FAULT_4_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_FAULT_PROTECT] && led_mask[LED_FAULT_PROTECT] ) {
		HAL_GPIO_WritePin(LED_FAULT_5_GPIO_Port, LED_FAULT_5_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_5_GPIO_Port, LED_FAULT_5_Pin, GPIO_PIN_RESET);
	}

	if (led_state[LED_FAULT_FAN] && led_mask[LED_FAULT_FAN] ) {
		HAL_GPIO_WritePin(LED_FAULT_6_GPIO_Port, LED_FAULT_6_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_6_GPIO_Port, LED_FAULT_6_Pin, GPIO_PIN_RESET);
	}

	return;
}

