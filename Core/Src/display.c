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

	uint16_t led_state = 0; 		//the current state of each LED. 0 = off, 1 = on
	uint16_t led_mask =  0xffff;  	//the mask of each LED. 0 = off, 1 = on.
									//each led is represented by one bit.
									//If the mask and state bits are both 1 the led will be turned on.
									//otherwise it will be turned off.

	const uint16_t led_state_all_on = 0xffff;
	const uint16_t led_state_all_off	= 0;

	uint16_t set_bits = 0;
	uint16_t clear_bits = 0xffff;

	initialize_leds();

	led_state = led_state | LED_POWER_ON;


	for (;;)
	{

		if (uxQueueMessagesWaiting(xDisplayQueue) > 0) {
			xStatus = xQueueReceive(xDisplayQueue, &displayMessage, 0);

			if (xStatus == pdPASS) {

				switch (displayMessage.displayCommand) {
				case SET_LED_STATE:
					led_state = (led_state & ~displayMessage.modify_mask) | (displayMessage.modify_mask & displayMessage.new_values);
					break;
				case SET_LED_MASK:
					led_mask = (led_mask & ~displayMessage.modify_mask) | (displayMessage.modify_mask & displayMessage.new_values);
					break;

				case SET_FADE:
					//todo: implement PWM adjustment for fade level of user leds

					break;

				case BLINK_MASK:
					for (int i = 0; i < NUM_BLINKS; i++) {
						set_led_states(led_state_all_on, led_mask);
						osDelay(50);
						set_led_states(led_state_all_off, led_mask);
						osDelay(50);
					}


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


void set_led_states(uint16_t led_state, uint16_t led_mask)
{
	uint16_t led_on = led_state & led_mask;
	if (led_on & LED_POWER_ON) {
		HAL_GPIO_WritePin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_STATUS_1_GPIO_Port, LED_STATUS_1_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_STANDBY_ON) {
		HAL_GPIO_WritePin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_STATUS_2_GPIO_Port, LED_STATUS_2_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_DSP_1) {
		HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_DSP_2) {
		HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_DSP_3) {
		HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_DSP_4) {
		HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_MONITOR_OVERTEMP) {
		HAL_GPIO_WritePin(LED_MONITOR_1_GPIO_Port, LED_MONITOR_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_1_GPIO_Port, LED_MONITOR_1_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_MONITOR_CLIP) {
		HAL_GPIO_WritePin(LED_MONITOR_2_GPIO_Port, LED_MONITOR_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_2_GPIO_Port, LED_MONITOR_2_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_MONITOR_PROTECT) {
		HAL_GPIO_WritePin(LED_MONITOR_3_GPIO_Port, LED_MONITOR_3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_3_GPIO_Port, LED_MONITOR_3_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_MONITOR_FAN) {
		HAL_GPIO_WritePin(LED_MONITOR_4_GPIO_Port, LED_MONITOR_4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_MONITOR_4_GPIO_Port, LED_MONITOR_4_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_FAULT_HF_TEMP) {
		HAL_GPIO_WritePin(LED_FAULT_1_GPIO_Port, LED_FAULT_1_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_1_GPIO_Port, LED_FAULT_1_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_FAULT_MF_TEMP) {
		HAL_GPIO_WritePin(LED_FAULT_2_GPIO_Port, LED_FAULT_2_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_2_GPIO_Port, LED_FAULT_2_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_FAULT_LF_TEMP) {
		HAL_GPIO_WritePin(LED_FAULT_3_GPIO_Port, LED_FAULT_3_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_3_GPIO_Port, LED_FAULT_3_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_FAULT_CLIP) {
		HAL_GPIO_WritePin(LED_FAULT_4_GPIO_Port, LED_FAULT_4_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_4_GPIO_Port, LED_FAULT_4_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_FAULT_PROTECT) {
		HAL_GPIO_WritePin(LED_FAULT_5_GPIO_Port, LED_FAULT_5_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_5_GPIO_Port, LED_FAULT_5_Pin, GPIO_PIN_RESET);
	}

	if (led_on & LED_FAULT_FAN) {
		HAL_GPIO_WritePin(LED_FAULT_6_GPIO_Port, LED_FAULT_6_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_FAULT_6_GPIO_Port, LED_FAULT_6_Pin, GPIO_PIN_RESET);
	}

	return;
}

