/*
 * board.c
 *
 *  Created on: May 23, 2024
 *      Author: 23877
 */

#include "board.h"

Board board;

void Board_Init() {
	RE35_Config config1 = { 0, 1, -1, 0 };
	board.x_motor = RE35_Motor_Create(config1);
	RE35_Config config2 = { 0, 2, -1, 0 };
	board.y_motor = RE35_Motor_Create(config2);
	RE35_Config config3 = { 0, 3, -1, 0 };
	board.z_motor = RE35_Motor_Create(config3);

	RE35_Motor_Init(board.x_motor, POS);
	RE35_Motor_Init(board.y_motor, POS);
	RE35_Motor_Init(board.z_motor, POS);
}

