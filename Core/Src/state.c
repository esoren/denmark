/*
 * state.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Eric
 */


#include "state.h"

static enum state current_state = ALL_ON;

void transition() {

    switch(current_state) {
        case ALL_ON:
           	current_state = STATUS;
            break;
        case STATUS:
            current_state = STATUS_MODE;
            break;
        case STATUS_MODE:
            current_state = STATUS_MODE_MON;
            break;
        case STATUS_MODE_MON:
            current_state = STATUS_MODE_FAULT;
            break;
        case STATUS_MODE_FAULT:
            current_state = STATUS_FAULT;
            break;
        case STATUS_FAULT:
            current_state = ALL_OFF;
            break;
        case ALL_OFF:
            current_state = ALL_ON;
            break;

    }
}
