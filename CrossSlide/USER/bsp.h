#ifndef BSP_H_
#define BSP_H_

#include "can.h"
#include "cmsis_os.h"

typedef struct {
	CAN_HandleTypeDef *handle;
	CAN_TxHeaderTypeDef TxHeader;      //���;��
	CAN_RxHeaderTypeDef RxHeader;      //���վ��
} CAN_info;

#pragma pack(1)
typedef struct {
	int16_t target_vel[3];// ���յ��ٶȣ�rpm�����յ�λ�ã�mm
	float target_pos[3];
} Cmd;
#pragma pack()

extern osMessageQId JogCmdQueueHandle;

void BSP_Init(void);
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
void CAN_Filter_Init(void);   //���������ú���
void CAN_Send_Msg(uint8_t can_ind, uint8_t id, uint8_t *msg, uint8_t len);  //���ݷ��ͺ���
void Send_Cmd(void);

#endif /* BSP_H_ */
