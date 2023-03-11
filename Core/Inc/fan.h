/*
 * fan.h
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */

#ifndef INC_FAN_H_
#define INC_FAN_H_

void StartFanTask(void const *argument);
uint16_t set_fan_rpm(uint16_t rpm);

#define FAN_PERIOD_CYCLES 		3360
#define MIN_DUTY_CYCLE_PERCENT 	20
#define MAX_FAN_RPM				3000
#define NOMINAL_FAN_RPM			1500
#define RPM_ADJ					50

#define DEFAULT_FAN_RPM			1500
#define FAN_FAULT_MARGIN		200 //fault if the fan RPM is this many RPMs below the target

#define FAN_SAMPLE_TIME_MS 200
#define FAN_STARTUP_MONITOR_TIME_MS 100

enum {
	FAN_MONITOR_START,
	FAN_MONITOR_STOP,
	FAN_SET_SPEED_MAX,
	FAN_SET_SPEED_NOMINAL,
	FAN_SET_SPEED_OFF
};

#define TIMER_CLK				84000000
#endif /* INC_FAN_H_ */
