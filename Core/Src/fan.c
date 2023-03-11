/*
 * fan.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "fan.h"
#include "display.h"
#include "tim.h"
#include "fault.h"

extern QueueHandle_t xDisplayQueue;

QueueHandle_t xFanQueue;
volatile uint32_t fan_rpm; //global variable continually set by the TIMER5 IC callback


void StartFanTask(void const *argument) {

	uint16_t target_fan_rpm = set_fan_rpm(0);

	//start PWM output timer
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);


	//start timer for input compare frequency measurement
	HAL_TIM_Base_Start_IT(&htim5);
	HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_4);

	osDelay(100); //give time for fan RPM measurement to stabilize

	displayMessage_t displayMessage;

	uint8_t fanMessage;
	uint8_t fan_monitor_is_running = 0; //if this flag is set, the fan monitor will run


	BaseType_t xStatus;

	for (;;)
	{

		if (uxQueueMessagesWaiting(xFanQueue) > 0) {
			xStatus = xQueueReceive(xFanQueue, &fanMessage, 0);
			if (xStatus == pdPASS) {
				switch(fanMessage) {
				case FAN_MONITOR_START:
					fan_monitor_is_running = 1;
					break;
				case FAN_MONITOR_STOP:
					fan_monitor_is_running = 0;
					break;
				case FAN_SET_SPEED_MAX:
					target_fan_rpm = set_fan_rpm(MAX_FAN_RPM);
					break;
				case FAN_SET_SPEED_NOMINAL:
					target_fan_rpm = set_fan_rpm(NOMINAL_FAN_RPM);
					break;
				case FAN_SET_SPEED_OFF:
					target_fan_rpm = set_fan_rpm(0);
					break;
				default:
					//should not reach here, do nothing
					break;
				}


			}
		}

		//todo: consider modifying this to only send a display message if the fan state has changed
		if(fan_monitor_is_running) {
			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_FAN;

			if( (target_fan_rpm > FAN_FAULT_MARGIN) && (fan_rpm < (target_fan_rpm - FAN_FAULT_MARGIN)) ) {
				displayMessage.new_values = 0xffff;
				xQueueSend(xDisplayQueue, &displayMessage, 0);

				set_clear_fault_flags(FAULT_FAN, 1);

			} else {
				displayMessage.new_values = 0x0000;
				xQueueSend(xDisplayQueue, &displayMessage, 0);
			}

			osDelay(FAN_SAMPLE_TIME_MS);
		} else {
			osDelay(FAN_STARTUP_MONITOR_TIME_MS);
		}
	}


}


uint16_t set_fan_rpm(uint16_t rpm) {

	if(rpm == 0) {
		TIM1->CCR1 = 0;  //handle the special case of turning off the fan first
		return 0;
	}



	float duty_cycle_percent = 100;
	uint16_t pwm_on_cycles = 0;

	if(rpm > MAX_FAN_RPM) {
		rpm = MAX_FAN_RPM;

	}

	duty_cycle_percent = (float)(rpm + RPM_ADJ) / MAX_FAN_RPM * 100.0;

	if(duty_cycle_percent < MIN_DUTY_CYCLE_PERCENT) {
		duty_cycle_percent = MIN_DUTY_CYCLE_PERCENT; //todo: adjust rpm set value in this case
	}

	pwm_on_cycles = duty_cycle_percent / 100 * FAN_PERIOD_CYCLES;

	TIM1->CCR1 = pwm_on_cycles;
	return rpm;

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
	static uint32_t CCR4_PREV = 0;
	static uint32_t CCR4_CURR = 0;
	uint32_t cycles_per_period = 0;
	static uint32_t prev_cycles_per_period = 0;


	CCR4_CURR = TIM5->CCR4;

	if(CCR4_CURR > CCR4_PREV) {   //in case the timer overflows, use the previous value
		cycles_per_period = CCR4_CURR - CCR4_PREV;
	} else {
		cycles_per_period = prev_cycles_per_period;
	}
	prev_cycles_per_period = cycles_per_period;

	CCR4_PREV = CCR4_CURR;

	fan_rpm = TIMER_CLK * 60.0 / 2 / cycles_per_period;

	return;

}


