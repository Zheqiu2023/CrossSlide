#ifndef MAXON_RE35_H_
#define MAXON_RE35_H_

#include <stdbool.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "msg.h"

#define RE35_REDUCTION_RATIO 35	// 减速比
#define ENCODER_LINES_NUM 2000	// 编码器线数
#define PWM_LIM 5000	// pwm限制值
#define LEAD 5	// 丝杠导程（mm）

typedef enum {
	VEL = 0X03,     // 速度模式
	POS = 0X04,		// 位置模式
	VEL_POS = 0X05  // 速度位置模式
} RE35_Run_Mode;

typedef struct {
	uint8_t can_ind;
	uint8_t driver_id;
	int8_t dir; // 旋转方向，顺时针为负，逆时针为正
	uint8_t period;
} RE35_Config;

typedef struct {
	uint8_t cmd_id;
	RE35_Run_Mode mode;
	uint8_t tx_data[8];		// 发送的数据
} RE35_Cmd;

typedef struct {
	int16_t current;	// 当前电流（mA）
	int16_t speed;      // 当前角速度（rpm）
	int32_t angle;      // 当前角度值（qc）
} RE35_Data;

typedef struct {
	RE35_Config config;
	RE35_Cmd cmd;
	RE35_Data data;

	bool reset_flag;
} RE35_Motor;

extern RE35_Motor re35_motor[3];
extern MotorFBData fb_data;

void RE35_Motor_Create(void);
void RE35_Motor_Init(RE35_Motor *obj, RE35_Run_Mode mode);
void RE35_Motor_SetCmd(RE35_Motor *obj, RE35_Run_Mode mode, int32_t speed, int32_t angle);
void RE35_Motor_RecvData_Process(RE35_Motor *obj, uint32_t recv_id, uint8_t *data);
void RE35_Motor_Send(RE35_Motor *obj);

int32_t Convert_Pos(float pos);

#endif /* MAXON_RE35_H_ */
