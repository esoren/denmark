/*
 * dsp.h
 *
 *  Created on: Mar 4, 2023
 *      Author: Eric
 */

#ifndef INC_DSP_H_
#define INC_DSP_H_

enum E_dsp_state {
    DSP_MODE_1,
    DSP_MODE_2,
    DSP_MODE_3,
    DSP_MODE_4
};

void next_dsp_mode();
void set_dsp_mode(int new_dsp_state);
void send_ir_dsp_command(int current_dsp_state);

#endif /* INC_DSP_H_ */
