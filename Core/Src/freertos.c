/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "display.h"
#include "queue.h"
#include "input.h"
#include "temperature.h"
#include "dsp.h"
#include "fan.h"

extern QueueHandle_t xDisplayQueue;
extern QueueHandle_t xTemperatureQueue;
extern QueueHandle_t xFanQueue;

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
osThreadId displayTaskHandle;
osThreadId inputTaskHandle;
osThreadId temperatureTaskHandle;
osThreadId fanTaskHandle;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

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


	//QUEUES
	xDisplayQueue = xQueueCreate(30, sizeof(displayMessage_t));
	xTemperatureQueue = xQueueCreate(30, sizeof(uint8_t));
	xFanQueue = xQueueCreate(30, sizeof(uint8_t));

	//TASKS
	osThreadDef(displayTask, StartDisplayTask, osPriorityNormal, 0, 128);
	displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

	osThreadDef(inputTask, StartInputTask, osPriorityNormal, 0, 128);
	inputTaskHandle = osThreadCreate(osThread(inputTask), NULL);

	osThreadDef(temperatureTask, StartTemperatureTask, osPriorityNormal, 0, 128);
	temperatureTaskHandle = osThreadCreate(osThread(temperatureTask), NULL);

	osThreadDef(fanTask, StartFanTask, osPriorityNormal, 0, 128);
	fanTaskHandle = osThreadCreate(osThread(fanTask), NULL);

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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

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
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  uint8_t toggle = 0;

  displayMessage_t displayMessage;

  uint8_t temperatureMessage = TEMPERATURE_MONITOR_START;
  xQueueSend(xTemperatureQueue, &temperatureMessage, 0);

  uint8_t fanMessage = FAN_MONITOR_START;
  xQueueSend(xFanQueue, &fanMessage, 0);

  set_dsp_mode(DSP_MODE_1);

  for(;;)
  {
	  toggle = (toggle + 1) % 2;



	  displayMessage.displayCommand = SET_LED_STATE;
	  displayMessage.modify_mask = 0x0000;


	  if(toggle) {
		  displayMessage.new_values = 0xaaaa;
	  } else {
		  displayMessage.new_values = 0x5555;
	  }
	  xQueueSend(xDisplayQueue, &displayMessage, 0);

	  osDelay(400);





  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
