/*
 * fault.h
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */

#ifndef INC_FAULT_H_
#define INC_FAULT_H_

// Define the available flags using an enum
typedef enum {
    FAULT_HF_TEMP,
    FAULT_MF_TEMP,
    FAULT_LF_TEMP,
    FAULT_CLIP,
    FAULT_PROTECT,
    FAULT_FAN,
    FAULT_ALL // A special flag to clear all flags
} fault_flag_t;

// Function prototype
void set_clear_fault_flags(fault_flag_t flag, int state);
#endif /* INC_FAULT_H_ */
