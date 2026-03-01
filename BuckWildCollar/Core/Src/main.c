/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "adc.h"
#include "icache.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

// stdlib Includes
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

typedef enum{
	idle,
	button_pressed,
	active,
	orientation_receive,
	adc_receive,
} Microcontroller_State;

// Useful Variables
uint8_t uart1_tx_buffer[BUFFER_SIZE] = "Error: Initial TX1 String\r\n";
uint8_t uart1_rx_buffer[BUFFER_SIZE] = "Error: Initial RX1 String\r\n";
uint8_t uart2_tx_buffer[BUFFER_SIZE] = "Error: Initial TX2 String\r\n";
uint8_t uart2_rx_buffer[BUFFER_SIZE] = "Error: Initial RX2 String\r\n";

// ADC
// 14-bit = 16384 MAX
static int adc_resistor; // Display value directly from the ADC
static int force_resistor; // Display pseudo "resistance value"

//BNO055 Commands
static uint8_t[4] =


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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */








static void print(UART_HandleTypeDef *huart, const char *fmt, ...);

static void print(UART_HandleTypeDef *huart, const char *fmt, ...) {
  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(huart, (uint8_t *)buffer, len, -1);
  return;
}

static int Get_ADC(){
	//TODO: Add Calibration for Max / Min
	HAL_ADC_Start(&hadc1); // Start polling
	HAL_ADC_PollForConversion(&hadc1, 1); // Check Timeout or switch to IT based
	adc_resistor = HAL_ADC_GetValue(&hadc1); // Take ADC Value
	HAL_ADC_Stop(&hadc1); // Stop polling

	force_resistor = adc_resistor; // TODO: Convert from adc_resistor to "resistance" value.
	return force_resistor;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  return; // ADC Callback for Later Use??
}




//static void uart_rx(UART_HandleTypeDef *huart, uint8_t *buffer){
//	HAL_UART_Receive_IT(huart, (uint8_t *)buffer, BUFFER_SIZE);
//}
//
//static void uart_tx(UART_HandleTypeDef *huart, uint8_t *buffer){
//	HAL_UART_Transmit_IT(huart, (uint8_t *)buffer, BUFFER_SIZE);
//}







/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */





  while(1){


//	  uint8_t empty[BUFFER_SIZE] = "";
//	  memcpy(rx_buffer, empty, BUFFER_SIZE);
//	  HAL_UART_Receive (&huart2, rx_buffer, 12, 5);
//	  HAL_UART_Transmit(&huart1, rx_buffer, 12, 5);
//	  print(&huart1, "\r\n");
//	  HAL_Delay(20);

//	  uint8_t rx_buffer1[4] = {0xAA, 0x01, 0x2C, 02};
//	  //HAL_UART_Transmit(&huart1, rx_buffer1, 12, 5);
//	  HAL_UART_Transmit(&huart2, rx_buffer1, 12, 5);
//	  HAL_UART_Receive (&huart2, rx_buffer, 12, 5);
//	  HAL_UART_Transmit (&huart1, rx_buffer, 12, 5);
  }


  int button;
  int state = idle;

  while(1){
	  if (state == idle){
		  button = BSP_PB_GetState(BUTTON_USER);
		  if (button == 1){
			  button = 0;
			  state = button_pressed;
		  }
	  }
	  else if (state == button_pressed){
		  print(&huart1, "Button Activated\r\n");
		  state = orientation_receive;
	  }
	  else if (state == orientation_receive){
		  state = idle;
	  }
	  else if (state == adc_receive){
		  Get_ADC();
		  print(&huart1, "%d\r\n", force_resistor);
		  state = idle;
	  }

	  print(&huart1, "%d\r\n", state);
  }





















  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{

  /*
   * Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
   */
  HAL_PWREx_DisableUCPDDeadBattery();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM17 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  print(&huart1, "Error");
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
