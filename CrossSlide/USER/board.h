/*
 * board.h
 *
 *  Created on: May 23, 2024
 *      Author: 23877
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "bsp.h"
#include "maxon_re35.h"

typedef struct {
	RE35_Motor *x_motor;
	RE35_Motor *y_motor;
	RE35_Motor *z_motor;
} Board;

extern Board board;

void Board_Init(void);

#endif /* BOARD_H_ */
