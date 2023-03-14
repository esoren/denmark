/*
 * power.h
 *
 *  Created on: Mar 6, 2023
 *      Author: Eric
 */

#ifndef INC_POWER_H_
#define INC_POWER_H_


void StartPowerTask(void const *argument);
void pup_sequence();
void set_clear_amp_standby(uint8_t amplifier, uint8_t standby_state);
void set_clear_amp_mute(uint8_t amplifier, uint8_t mute_state);
uint8_t enter_power_state_amps_on(void);
uint8_t enter_power_state_standby(void);

#define DSP_POWERUP_DELAY_MS 4500

enum {
	POWER_STATE_PUP_SEQUENCE,
	POWER_STATE_STANDBY,
	POWER_STATE_AMPS_ON,
	POWER_STATE_OVERTEMP,
	POWER_STATE_OTHER_FAULT
};


enum {
	LF_AMP,
	MF_AMP,
	HF_AMP
};

enum {
	STANDBY_ON,
	STANDBY_OFF
};

enum {
	MUTE_ON,
	MUTE_OFF
};

enum {
	POWER_TOGGLE_STANDBY,
	POWER_OVERTEMP
};

#endif /* INC_POWER_H_ */
