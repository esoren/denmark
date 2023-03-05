/*
 * state.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Eric
 */

#include "cmsis_os.h"
#include "state.h"
#include "display.h"



extern QueueHandle_t xDisplayQueue;


void update_mask() {

	static enum state current_state = ALL_ON;

	displayMessage_t displayMessage;

	displayMessage.displayCommand = SET_LED_MASK;
	displayMessage.modify_mask = 0xffff;

    switch(current_state) {
        case ALL_ON:
           	displayMessage.new_values = 0xffff;
           	xQueueSend(xDisplayQueue, &displayMessage, 0);
           	current_state = STATUS;
            break;

        case STATUS:
        	displayMessage.new_values = 0;
        	displayMessage.new_values = displayMessage.new_values | (LED_POWER_ON | LED_STANDBY_ON);
        	xQueueSend(xDisplayQueue, &displayMessage, 0);
            current_state = STATUS_MODE;
            break;

        case STATUS_MODE:
        	displayMessage.new_values = 0;
        	displayMessage.new_values = displayMessage.new_values | (LED_POWER_ON | LED_STANDBY_ON | LED_DSP_1 | LED_DSP_2 | LED_DSP_3 | LED_DSP_4);
        	xQueueSend(xDisplayQueue, &displayMessage, 0);
        	current_state = STATUS_MODE_MON;
            break;

        case STATUS_MODE_MON:
        	displayMessage.new_values = 0;
            displayMessage.new_values = displayMessage.new_values | (LED_POWER_ON | LED_STANDBY_ON | LED_DSP_1 | LED_DSP_2 | LED_DSP_3 | LED_DSP_4
            													 | LED_MONITOR_OVERTEMP | LED_MONITOR_CLIP | LED_MONITOR_PROTECT | LED_MONITOR_FAN);
            xQueueSend(xDisplayQueue, &displayMessage, 0);
        	current_state = STATUS_MODE_FAULT;
            break;

        case STATUS_MODE_FAULT:
        	displayMessage.new_values = 0;
        	displayMessage.new_values = displayMessage.new_values | (LED_POWER_ON | LED_STANDBY_ON | LED_DSP_1 | LED_DSP_2 | LED_DSP_3 | LED_DSP_4
        	   													 | LED_FAULT_HF_TEMP | LED_FAULT_MF_TEMP | LED_FAULT_LF_TEMP | LED_FAULT_CLIP
																 | LED_FAULT_PROTECT | LED_FAULT_FAN);
        	xQueueSend(xDisplayQueue, &displayMessage, 0);
            current_state = ALL_OFF;
        	break;

        case ALL_OFF:
        	displayMessage.new_values = 0;
        	xQueueSend(xDisplayQueue, &displayMessage, 0);
        	current_state = ALL_ON;
            break;

    }

    displayMessage.displayCommand = BLINK_MASK;
    displayMessage.modify_mask = 0;
    displayMessage.new_values = 0;
    xQueueSend(xDisplayQueue, &displayMessage, 0);

}
