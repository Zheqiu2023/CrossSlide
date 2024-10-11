#include "bsp.h"

#include <math.h>

#include "can.h"
#include "main.h"
#include "maxon_re35.h"
#include "string.h"
#include "usart.h"
#include "usbd_cdc_if.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"

CAN_info CAN1_port;

#define UART_BUFFER_SIZE 255
uint8_t rx_buff[UART_BUFFER_SIZE];
Cmd recv_cmd;
uint8_t data_length;

void BSP_Init() {
    // 使能串口空闲中断
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    // 开启DMA接收
    HAL_UART_Receive_DMA(&huart4, rx_buff, UART_BUFFER_SIZE);

    CAN1_port.handle = &hcan1;
}

/* 放在"stm32f4xx_it.c"里形如"void USART2_IRQHandler(void)"类的函数中，只要用了DMA接收的串口都放 */
void USER_UART_IRQHandler(UART_HandleTypeDef *huart) {
    // 判断是否进入空闲中断
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET) {
        __HAL_UART_CLEAR_IDLEFLAG(huart);  // 清楚空闲中断标志
        HAL_UART_DMAStop(huart);           // 暂停本次DMA传输，进行数据处理
        data_length = UART_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);  // 计算接收到的数据长度
        // 数据处理
        if (huart == &huart4) {
            memcpy(&recv_cmd, &rx_buff[6], sizeof(Cmd));
			osMessagePut(JogCmdQueueHandle, (uint32_t)&recv_cmd, 0);
        }
        HAL_UART_Receive_DMA(huart, rx_buff, UART_BUFFER_SIZE);  // 重启DMA接收
    }
}

/* CAN过滤器初始化 */
void CAN_Filter_Init() {
    CAN_FilterTypeDef fcan;

    fcan.FilterBank = 0;
    fcan.FilterMode = CAN_FILTERMODE_IDMASK;
    fcan.FilterScale = CAN_FILTERSCALE_32BIT;

    fcan.FilterIdHigh = 0;
    fcan.FilterIdLow = 0;
    fcan.FilterMaskIdHigh = 0;
    fcan.FilterMaskIdLow = 0;
    fcan.FilterFIFOAssignment = CAN_RX_FIFO0;
    fcan.FilterActivation = ENABLE;
    fcan.SlaveStartFilterBank = 14;

    /* Filter configuration */
    if (HAL_CAN_ConfigFilter(&hcan1, &fcan) != HAL_OK) Error_Handler();
    /*enable can interrupt*/
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) Error_Handler();
    /* Start CAN */
    if (HAL_CAN_Start(&hcan1) != HAL_OK) Error_Handler();
}

/* CAN发送数据函数 */
void CAN_Send_Msg(uint8_t can_ind, uint8_t id, uint8_t *msg, uint8_t len) {
    CAN1_port.TxHeader.StdId = id;
    CAN1_port.TxHeader.ExtId = 0;
    CAN1_port.TxHeader.IDE = CAN_ID_STD;
    CAN1_port.TxHeader.RTR = CAN_RTR_DATA;
    CAN1_port.TxHeader.DLC = len;

    //	while(HAL_CAN_GetTxMailboxesFreeLevel(CAN1_port.handle) == 0) {}

    /*找到空的发送邮箱，把数据发送出去*/
    if (HAL_CAN_AddTxMessage(CAN1_port.handle, &CAN1_port.TxHeader, msg, (uint32_t *)CAN_TX_MAILBOX0) != HAL_OK) {
        if (HAL_CAN_AddTxMessage(CAN1_port.handle, &CAN1_port.TxHeader, msg, (uint32_t *)CAN_TX_MAILBOX1) != HAL_OK) {
            if (HAL_CAN_AddTxMessage(CAN1_port.handle, &CAN1_port.TxHeader, msg, (uint32_t *)CAN_TX_MAILBOX2) != HAL_OK)
                Error_Handler();
        }
    }
}

/* CAN接收中断回调函数 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    uint8_t rxdata[8] = {0};
    if (hcan->Instance == CAN1) {
        HAL_CAN_GetRxMessage(CAN1_port.handle, CAN_RX_FIFO0, &CAN1_port.RxHeader, rxdata);
        uint8_t temp_id = (CAN1_port.RxHeader.StdId >> 4) & 0x0f;
        for (int i = 0; i < 3; ++i) {
            if (temp_id == re35_motor[i].config.driver_id) {
                RE35_Motor_RecvData_Process(&re35_motor[i], CAN1_port.RxHeader.StdId, rxdata);
				break;
            }
        }
    }
}

