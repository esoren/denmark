/*
 * power.h
 *
 *  Created on: Mar 6, 2023
 *      Author: Eric
 */

#ifndef INC_POWER_H_
#define INC_POWER_H_

enum {
	POWER_STATE_PUP_SEQUENCE,
	POWER_STATE_STANDBY,
	POWER_STATE_AMPS_ON,
	POWER_STATE_OVERTEMP,
	POWER_STATE_OTHER_FAULT
};

void StartPowerTask(void const *argument);

enum {
	AMP_LF,
	AMP_MF,
	AMP_HF
};

enum {
	AMP_IN_STANDBY,
	AMP_POWERED_UP
};

#endif /* INC_POWER_H_ */
