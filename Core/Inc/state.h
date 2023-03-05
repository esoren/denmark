/*
 * state.h
 *
 *  Created on: Mar 3, 2023
 *      Author: Eric
 */

#ifndef INC_STATE_H_
#define INC_STATE_H_

enum state {
    ALL_ON,
    ALL_OFF,
    STATUS,
    STATUS_MODE,
    STATUS_MODE_MON,
    STATUS_MODE_FAULT,
    STATUS_FAULT
};

void transition(void);

#endif /* INC_STATE_H_ */
