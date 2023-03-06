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

extern QueueHandle_t xDisplayQueue;


void StartFanTask(void const *argument) {

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	set_fan_rpm(DEFAULT_FAN_RPM);

	for (;;)
	{


		osDelay(400);

	}


}


void set_fan_rpm(uint16_t rpm) {

	rpm = rpm + RPM_ADJ; //apply an experimental offset (in Hz) to the target RPM

	float duty_cycle_percent = 100;
	uint16_t pwm_on_cycles = 0;

	if(rpm > MAX_FAN_RPM) {
		rpm = MAX_FAN_RPM;
	}

	duty_cycle_percent = (float)rpm / MAX_FAN_RPM * 100.0;

	if(duty_cycle_percent < MIN_DUTY_CYCLE_PERCENT) {
		duty_cycle_percent = MIN_DUTY_CYCLE_PERCENT;
	}

	pwm_on_cycles = duty_cycle_percent / 100 * FAN_PERIOD_CYCLES;

	TIM1->CCR1 = pwm_on_cycles;


}
