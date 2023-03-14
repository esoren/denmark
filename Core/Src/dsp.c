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
#include "fram.h"
#include "infrared.h"
#include "gpio.h"
#include "power.h"

extern QueueHandle_t xDisplayQueue;


volatile enum E_dsp_state current_dsp_state = DSP_MODE_1;


//todo: consider combining both of these functions so that the state can be stored as a static variable inside of the function
//todo: maybe this should be a task?

void next_dsp_mode() {


 	switch(current_dsp_state) {
        case DSP_MODE_1:
        	set_dsp_mode(DSP_MODE_2);
            break;

        case DSP_MODE_2:
        	set_dsp_mode(DSP_MODE_3);
            break;

        case DSP_MODE_3:
        	set_dsp_mode(DSP_MODE_4);
            break;
        case DSP_MODE_4:
        	set_dsp_mode(DSP_MODE_1);
        	break;


    }


}


void set_dsp_mode(uint8_t new_dsp_state) {


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



 	uint8_t hf_amp_mute = get_amp_mute_state(HF_AMP); //store the mute and standby states
 	uint8_t mf_amp_mute = get_amp_mute_state(MF_AMP);
 	uint8_t lf_amp_standby = get_amp_standby_state(LF_AMP);

 	set_clear_amp_mute(HF_AMP, MUTE_ON); //force into mute/standby while loading DSP
 	set_clear_amp_mute(MF_AMP, MUTE_ON);
 	set_clear_amp_standby(LF_AMP, STANDBY_ON);


 	send_ir_dsp_command(current_dsp_state);
 	fram_write_dsp_mode(current_dsp_state);
 	wait_for_dsp_load_blocking(current_dsp_state);

 	set_clear_amp_standby(LF_AMP, lf_amp_standby);  //restore states to previous values
 	set_clear_amp_mute(HF_AMP, hf_amp_mute);
 	set_clear_amp_mute(MF_AMP, mf_amp_mute);
}

/* The DSP functions are implemented on a minidsp 2x4 HD.
 * The 2x4 HD supports four different DSP modes.
 * The DSP mode can only be configured using an IR remote control, there is no other digital interface for configuration.
 * This function generates a waveform to emulate the MiniDSP remote and request a specific mode.
 * There is no way to readback the state of the minidsp, so this should be called anytime the state is not known
 * for instance, at powerup.
 */
void send_ir_dsp_command(int current_dsp_state) {

	switch(current_dsp_state) {
	case DSP_MODE_1:
		ir_send_message(0, IR_CONFIG_1);
		break;
	case DSP_MODE_2:
		ir_send_message(0, IR_CONFIG_2);
		break;
	case DSP_MODE_3:
		ir_send_message(0, IR_CONFIG_3);
		break;
	case DSP_MODE_4:
		ir_send_message(0, IR_CONFIG_4);
		break;
	}


	return;
}


void wait_for_dsp_load_blocking(int current_dsp_state) {

	HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_RESET);


	for (int i = 0; i < DSP_LOAD_NUM_BLINKS; i++) {
		switch(current_dsp_state) {
			case DSP_MODE_1:
				HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_RESET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				HAL_GPIO_WritePin(LED_MODE_1_GPIO_Port, LED_MODE_1_Pin, GPIO_PIN_SET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				break;
			case DSP_MODE_2:
				HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_RESET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				HAL_GPIO_WritePin(LED_MODE_2_GPIO_Port, LED_MODE_2_Pin, GPIO_PIN_SET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				break;
			case DSP_MODE_3:
				HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_RESET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				HAL_GPIO_WritePin(LED_MODE_3_GPIO_Port, LED_MODE_3_Pin, GPIO_PIN_SET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				break;
			case DSP_MODE_4:
				HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_RESET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				HAL_GPIO_WritePin(LED_MODE_4_GPIO_Port, LED_MODE_4_Pin, GPIO_PIN_SET);
				HAL_Delay(DSP_LOAD_BLINK_PERIOD_MS/2);
				break;
			}
	}



}
