/*
 * fault.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */

#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "fault.h"
#include "display.h"

extern QueueHandle_t xDisplayQueue;

//todo: better function description
// Set or clear a specific flag, or clear all flags
void set_clear_fault_flags(fault_flag_t flag, int state) {

	static unsigned char fault_flags = 0; // Variable to hold the current state of the flags
	displayMessage_t displayMessage;


    if (flag == FAULT_ALL && state == 0) {
        fault_flags = 0; // Clear all flags
    } else if (state == 1) {
        fault_flags |= (1 << flag); // Set the flag
    } else if (state == 0) {
        fault_flags &= ~(1 << flag); // Clear the flag
    } else {
        // Invalid state value
        return;
    }

    displayMessage.displayCommand = SET_LED_STATE;
    displayMessage.modify_mask = (LED_FAULT_HF_TEMP | LED_FAULT_MF_TEMP | LED_FAULT_LF_TEMP | LED_FAULT_CLIP | LED_FAULT_PROTECT | LED_FAULT_FAN);
    displayMessage.new_values = 0;

    if(fault_flags & (1<<FAULT_HF_TEMP)) {
    	displayMessage.new_values |= LED_FAULT_HF_TEMP;
    }

    if(fault_flags & (1<<FAULT_MF_TEMP)) {
        	displayMessage.new_values |= LED_FAULT_MF_TEMP;
    }

    if(fault_flags & (1<<FAULT_LF_TEMP)) {
        displayMessage.new_values |= LED_FAULT_LF_TEMP;
    }

    if(fault_flags & (1<<FAULT_CLIP)) {
        displayMessage.new_values |= LED_FAULT_CLIP;
    }

    if(fault_flags & (1<<FAULT_PROTECT)) {
        displayMessage.new_values |= LED_FAULT_PROTECT;
    }

    if(fault_flags & (1<<FAULT_FAN)) {
        displayMessage.new_values |= LED_FAULT_FAN;
    }

    xQueueSend(xDisplayQueue, &displayMessage, 0);

}
