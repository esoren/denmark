/*
 * display.h
 *
 *  Created on: Mar 2, 2023
 *      Author: Eric
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


void initialize_leds();
void set_led_states(uint16_t led_state, uint16_t led_mask);


#define NUM_BLINKS 3   //number of blinks to use when displaying the LED mask

#define NUM_LEDS 16
enum {
	LED_POWER_ON 			=	1<<0,		//LED_STATUS_1
	LED_STANDBY_ON 			=	1<<1,		//LED_STATUS_2
	LED_DSP_1				=	1<<2,  		//LED_MODE_1
	LED_DSP_2				=   1<<3,		//LED_MODE_2
	LED_DSP_3 				=   1<<4,		//LED_MODE_3
	LED_DSP_4 				=	1<<5,		//LED_MODE_4
	LED_MONITOR_OVERTEMP	=   1<<6, 	  	//LED_MONITOR_1
	LED_MONITOR_CLIP 		=	1<<7,		//LED_MONITOR_2
	LED_MONITOR_PROTECT		=	1<<8,		//LED_MONITOR_3
	LED_MONITOR_FAN			=	1<<9,		//LED_MONITOR_4
	LED_FAULT_HF_TEMP		=	1<<10,		//LED_FAULT_1
	LED_FAULT_MF_TEMP		=	1<<11,		//LED_FAULT_2
	LED_FAULT_LF_TEMP		=	1<<12,		//LED_FAULT_3
	LED_FAULT_CLIP 			=   1<<13, 		//LED_FAULT_4
	LED_FAULT_PROTECT 		=	1<<14, 		//LED_FAULT_5
	LED_FAULT_FAN			= 	1<<15		//LED_FAULT_6
};



typedef enum {
	SET_LED_STATE=0,
	SET_LED_MASK,
	SET_FADE,
	BLINK_MASK
} EDisplayCommand;

typedef struct {
	EDisplayCommand displayCommand;
	uint16_t modify_mask;		//only the leds with the mask bit set will be modified
	uint16_t new_values;			//one bit per LED according to the LED enum
} displayMessage_t;

void StartDisplayTask(void const * argument);

#endif /* INC_DISPLAY_H_ */
