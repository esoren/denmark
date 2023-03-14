/*
 * dsp.h
 *
 *  Created on: Mar 4, 2023
 *      Author: Eric
 */

#ifndef INC_DSP_H_
#define INC_DSP_H_


#define DSP_LOAD_BLINK_PERIOD_MS 	1000
#define DSP_LOAD_NUM_BLINKS			4  //the product of these two values is the total time to wait for the DSP to load

enum E_dsp_state {
    DSP_MODE_1=1,
    DSP_MODE_2,
    DSP_MODE_3,
    DSP_MODE_4
};

void next_dsp_mode();
void set_dsp_mode(uint8_t new_dsp_state);
void send_ir_dsp_command(int current_dsp_state);
void wait_for_dsp_load_blocking(int current_dsp_state);

#endif /* INC_DSP_H_ */
