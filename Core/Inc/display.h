/*
 * display.h
 *
 *  Created on: Mar 2, 2023
 *      Author: Eric
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


void initialize_leds();
void set_led_states(uint8_t led_state[], uint8_t led_mask[]) ;


#define NUM_BLINKS 3   //number of blinks to use when displaying the LED mask

#define NUM_LEDS 16
enum {
	LED_POWER_ON=0, 		//LED_STATUS_1
	LED_STANDBY_ON,			//LED_STATUS_2
	LED_DSP_1, 				//LED_MODE_1
	LED_DSP_2,				//LED_MODE_2
	LED_DSP_3,				//LED_MODE_3
	LED_DSP_4,				//LED_MODE_4
	LED_MONITOR_OVERTEMP,   //LED_MONITOR_1
	LED_MONITOR_CLIP, 		//LED_MONITOR_2
	LED_MONITOR_PROTECT,	//LED_MONITOR_3
	LED_MONITOR_FAN,		//LED_MONITOR_4
	LED_FAULT_HF_TEMP,		//LED_FAULT_1
	LED_FAULT_MF_TEMP,		//LED_FAULT_2
	LED_FAULT_LF_TEMP,		//LED_FAULT_3
	LED_FAULT_CLIP, 		//LED_FAULT_4
	LED_FAULT_PROTECT, 		//LED_FAULT_5
	LED_FAULT_FAN,			//LED_FAULT_6
	LED_NONE
};



typedef enum {
	SET_STATE=0,
	SET_MASK,
	SET_FADE,
	BLINK_MASK
} EDisplayCommand;

typedef struct {
	EDisplayCommand displayCommand;
	uint8_t new_states[NUM_LEDS];
} displayMessage_t;

void StartDisplayTask(void const * argument);

#endif /* INC_DISPLAY_H_ */
