#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "maxon_re35.h"
#include "bsp.h"
#include "utilities.h"


RE35_Motor re35_motor[3];

void RE35_Motor_Create(void) {	
	RE35_Config config[3] = {
	{ 0, 1, -1, 0 },
	{ 0, 2, -1, 0 },
	{ 0, 3, -1, 0 }
	};
	
	for(int i = 0; i < 3; ++i){
		re35_motor[i].config = config[i];
		RE35_Motor_Init(&re35_motor[i], VEL_POS);
	}
}

void RE35_Motor_Init(RE35_Motor *obj, RE35_Run_Mode mode) {
	obj->reset_flag = false;
	obj->cmd.mode = mode;
	// 发送复位指令
	obj->cmd.cmd_id = 0x000 | (obj->config.driver_id << 4);  // 复位指令（帧ID，由驱动器编号和功能序号决定）
	for (uint8_t i = 0; i < 8; ++i)
		obj->cmd.tx_data[i] = 0x55;
	RE35_Motor_Send(obj);
	HAL_Delay(500);
	// 发送模式选择指令
	obj->cmd.cmd_id = 0x001 | (obj->config.driver_id << 4);   // 模式选择指令
	obj->cmd.tx_data[0] = mode;  // 选择mode对应模式
	RE35_Motor_Send(obj);
	HAL_Delay(500);
	// 发送配置指令
	obj->cmd.cmd_id = 0x00A | (obj->config.driver_id << 4);  // 配置指令
	obj->cmd.tx_data[0] = 0x05;    // 以 5 毫秒为周期对外发送电流、速度、位置等信息
	obj->cmd.tx_data[1] = obj->config.period;  // 以 period 毫秒为周期对外发送CTL1/CTL2的电平状态
	RE35_Motor_Send(obj);
	HAL_Delay(500);
}

void RE35_Motor_SetCmd(RE35_Motor *obj, RE35_Run_Mode mode, int32_t speed, int32_t angle) {
	if (obj->cmd.mode != mode)
		RE35_Motor_Init(obj, mode);

	int32_t temp_speed, temp_angle;
	obj->cmd.tx_data[0] = (uint8_t) ((PWM_LIM >> 8) & 0xff);
	obj->cmd.tx_data[1] = (uint8_t) (PWM_LIM & 0xff);
	switch (mode) {
		case VEL:
			temp_speed = speed * obj->config.dir;
			obj->cmd.cmd_id = 0x004 | (obj->config.driver_id << 4);
			obj->cmd.tx_data[2] = (uint8_t) ((temp_speed >> 8) & 0xff);
			obj->cmd.tx_data[3] = (uint8_t) (temp_speed & 0xff);
			for (uint8_t i = 4; i < 8; ++i)
				obj->cmd.tx_data[i] = 0x55;
			break;
		case POS:
			obj->cmd.cmd_id = 0x005 | (obj->config.driver_id << 4);
			obj->cmd.tx_data[2] = 0x55;
			obj->cmd.tx_data[3] = 0x55;
			obj->cmd.tx_data[4] = (uint8_t) ((temp_angle >> 24) & 0xff);
			obj->cmd.tx_data[5] = (uint8_t) ((temp_angle >> 16) & 0xff);
			obj->cmd.tx_data[6] = (uint8_t) ((temp_angle >> 8) & 0xff);
			obj->cmd.tx_data[7] = (uint8_t) (temp_angle & 0xff);
			break;
		case VEL_POS:
			temp_angle = angle * obj->config.dir;
			obj->cmd.cmd_id = 0x006 | (obj->config.driver_id << 4);
			obj->cmd.tx_data[2] = (uint8_t) ((speed >> 8) & 0xff);
			obj->cmd.tx_data[3] = (uint8_t) (speed & 0xff);
			obj->cmd.tx_data[4] = (uint8_t) ((temp_angle >> 24) & 0xff);
			obj->cmd.tx_data[5] = (uint8_t) ((temp_angle >> 16) & 0xff);
			obj->cmd.tx_data[6] = (uint8_t) ((temp_angle >> 8) & 0xff);
			obj->cmd.tx_data[7] = (uint8_t) (temp_angle & 0xff);
			break;
		default:
			break;
	}
}

void RE35_Motor_Send(RE35_Motor *obj) {
	CAN_Send_Msg(obj->config.can_ind, obj->cmd.cmd_id, obj->cmd.tx_data, sizeof(obj->cmd.tx_data));
}

MotorFBData fb_data;
void RE35_Motor_RecvData_Process(RE35_Motor *obj, uint32_t recv_id, uint8_t *data) {
	uint8_t temp_id = recv_id & 0x0f;

	if (temp_id == 0x0b) {
		// 接收到电流、速度、位置等信息
//		obj->data.current = (data[0] << 8) | data[1];
//		obj->data.speed = (data[2] << 8) | data[3];
//		obj->data.angle = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
		
		int id = (recv_id-0x0b)>>4;
		fb_data.vel[id-1] = (data[2] << 8) | data[3];
		fb_data.pos[id-1] = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
		
	} else if (temp_id == 0x0c && data[0] == 0x01) {
		// 接收到驱动器CTL1/CTL2的电平状态(有限位开关)
		obj->reset_flag = true;
		RE35_Motor_SetCmd(obj, obj->cmd.mode, 0, 0);
		RE35_Motor_Send(obj);
	}
}

int32_t Convert_Pos(float pos) {
	return round(pos * RE35_REDUCTION_RATIO * ENCODER_LINES_NUM / LEAD);	// mm转换为qc
}
