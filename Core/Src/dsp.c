/*
 * dsp.c
 *
 *  Created on: Mar 4, 2023
 *      Author: Eric
 */

#include "cmsis_os.h"
#include "dsp.h"
#include "display.h"
#include "freertos.h"
#include "queue.h"

extern QueueHandle_t xDisplayQueue;

volatile enum E_dsp_state current_dsp_state = DSP_MODE_1;


//todo: consider combining both of these functions so that the state can be stored as a static variable inside of the function
//todo: maybe this should be a task?

void next_dsp_mode() {


	displayMessage_t displayMessage;
	displayMessage.displayCommand = SET_LED_STATE;
	displayMessage.modify_mask = 0 | (LED_DSP_1 | LED_DSP_2 | LED_DSP_3 | LED_DSP_4);


 	switch(current_dsp_state) {
        case DSP_MODE_1:
        	displayMessage.new_values = LED_DSP_2;
            current_dsp_state = DSP_MODE_2;
            break;

        case DSP_MODE_2:
        	displayMessage.new_values = LED_DSP_3;
            current_dsp_state = DSP_MODE_3;
            break;

        case DSP_MODE_3:
        	displayMessage.new_values = LED_DSP_4;
            current_dsp_state = DSP_MODE_4;
            break;
        case DSP_MODE_4:

        	displayMessage.new_values = LED_DSP_1;
            current_dsp_state = DSP_MODE_1;
            break;
    }

 	xQueueSend(xDisplayQueue, &displayMessage, 0);
 	send_ir_dsp_command(current_dsp_state);

}


void set_dsp_mode(E_dsp_state new_dsp_state) {


	displayMessage_t displayMessage;
	displayMessage.displayCommand = SET_LED_STATE;
	displayMessage.modify_mask = 0 | (LED_DSP_1 | LED_DSP_2 | LED_DSP_3 | LED_DSP_4);


 	switch(new_dsp_state) {
        case DSP_MODE_1:
        	displayMessage.new_values = LED_DSP_1;
            current_dsp_state = DSP_MODE_1;
            break;

        case DSP_MODE_2:
        	displayMessage.new_values = LED_DSP_2;
            current_dsp_state = DSP_MODE_2;
            break;

        case DSP_MODE_3:
        	displayMessage.new_values = LED_DSP_3;
            current_dsp_state = DSP_MODE_3;
            break;

        case DSP_MODE_4:
        	displayMessage.new_values = LED_DSP_4;
            current_dsp_state = DSP_MODE_4;
            break;
    }

 	xQueueSend(xDisplayQueue, &displayMessage, 0);
 	send_ir_dsp_command(current_dsp_state);

}

/* The DSP functions are implemented on a minidsp 2x4 HD.
 * The 2x4 HD supports four different DSP modes.
 * The DSP mode can only be configured using an IR remote control, there is no other digital interface for configuration.
 * This function generates a waveform to emulate the MiniDSP remote and request a specific mode.
 * There is no way to readback the state of the minidsp, so this should be called anytime the state is not known
 * for instance, at powerup.
 */
void send_ir_dsp_command(E_dsp_state current_dsp_state) {
	//todo: implement IR send functionality here
	return;
}
