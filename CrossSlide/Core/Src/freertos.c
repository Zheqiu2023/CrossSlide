/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "msg.h"
#include "usart.h"
#include "utilities.h"
#include "bsp.h"
#include "maxon_re35.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId MotorDataTaskHandle;
osThreadId JogTaskHandle;
osMessageQId JogCmdQueueHandle;
osMessageQId MotorDataQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void ProcessMotorDataTask(void const * argument);
void StartJogTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of JogCmdQueue */
  osMessageQDef(JogCmdQueue, 16, uint32_t);
  JogCmdQueueHandle = osMessageCreate(osMessageQ(JogCmdQueue), NULL);

  /* definition and creation of MotorDataQueue */
  osMessageQDef(MotorDataQueue, 16, uint32_t);
  MotorDataQueueHandle = osMessageCreate(osMessageQ(MotorDataQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of MotorDataTask */
  osThreadDef(MotorDataTask, ProcessMotorDataTask, osPriorityBelowNormal, 0, 128);
  MotorDataTaskHandle = osThreadCreate(osThread(MotorDataTask), NULL);

  /* definition and creation of JogTask */
  osThreadDef(JogTask, StartJogTask, osPriorityNormal, 0, 128);
  JogTaskHandle = osThreadCreate(osThread(JogTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_ProcessMotorDataTask */
/**
* @brief Function implementing the MotorDataTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ProcessMotorDataTask */
void ProcessMotorDataTask(void const * argument)
{
  /* USER CODE BEGIN ProcessMotorDataTask */
	uint8_t data[22] = {0XAA, 0X55};
	data[20] = 0xcc; data[21] = 0x88;
  /* Infinite loop */
  for(;;)
  {
	taskENTER_CRITICAL();
		memcpy(&data[2], &fb_data, sizeof(fb_data));
		HAL_UART_Transmit_DMA(&huart4, (uint8_t*)data, 22);
	taskEXIT_CRITICAL();
	osDelay(5);
  }
  /* USER CODE END ProcessMotorDataTask */
}

/* USER CODE BEGIN Header_StartJogTask */
/**
* @brief Function implementing the JogTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartJogTask */
void StartJogTask(void const * argument)
{
  /* USER CODE BEGIN StartJogTask */
	Cmd *buf;
	osEvent evt;
  /* Infinite loop */
  for(;;)
  {
	evt = osMessageGet(JogCmdQueueHandle, osWaitForever);
	if (evt.status == osEventMessage) {
		buf = (Cmd*) evt.value.p;
		for (int i = 0; i < 3; ++i) {
			int32_t cmd_pos = Convert_Pos(buf->target_pos[i]);  // mm×ª»»Îªqc
			int16_t cmd_vel = buf->target_vel[i];
			RE35_Motor_SetCmd(&re35_motor[i], VEL_POS, cmd_vel, cmd_pos);
			RE35_Motor_Send(&re35_motor[i]);
		}
	}
  }
  /* USER CODE END StartJogTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
