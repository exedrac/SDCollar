/* USER CODE BEGIN Header */

/*
 *
 * BUCK WILD COLLAR
 * buckwildcollars.com
 *
 *
 * Perpetrators:
 * Paul Eickhoff
 *
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

#define BUFFER_SIZE 256
#define UART_DELAY 10

typedef enum{
	active,
	menu,
	sleep,

	gps_receive,
	gps_transmit,
	sense_bno055,
	motor_adjust,

	demo_active,
	demo_adc,
	demo_bno055,
	demo_maxm10s,
	demo_maxm10s_parsed,
	demo_motor,
	demo_lock,
	demo_stx3,
	demo_system,

	power_peripherals,
	power_sleep,
} Microcontroller_State;


/*
 * UART Buffers (The Flash Devourers!)
 * TODO: Balance buffer size with useful messages. We have 2MB flash total =~ 1.02MB
 */


uint8_t storage_buffer[BUFFER_SIZE] = "Initial Storage Buffer.";
const uint8_t empty_buffer[BUFFER_SIZE] = ""; // Used to clear buffers with memcpy. Also is an "impostor buffer".

uint8_t uart1_tx_buffer[BUFFER_SIZE] = "Initial TX1 String\r\n";
uint8_t uart1_rx_buffer[BUFFER_SIZE] = "Initial RX1 String\r\n";
uint8_t uart2_tx_buffer[BUFFER_SIZE] = "Initial TX2 String\r\n";
uint8_t uart2_rx_buffer[BUFFER_SIZE] = "Initial RX2 String\r\n";
uint8_t uart3_tx_buffer[BUFFER_SIZE] = "Initial TX3 String\r\n";
uint8_t uart3_rx_buffer[BUFFER_SIZE] = "Initial RX3 String\r\n";

uint8_t lpuart1_tx_buffer[BUFFER_SIZE] = "Initial lpTX1 String\r\n";
uint8_t lpuart1_rx_buffer[BUFFER_SIZE] = "Initial lpRX1 String\r\n";

//
uint8_t user_button = 0;
uint8_t reset = 0;

// Prototypes
static void print(const char *fmt, ...);
static void uart_send(char *buffer);
static void uart_receive(char *buffer);
static int Get_ADC();

// Functions

static void print(const char *fmt, ...) {
	/*
	 * Print message through USART1 channel
	 * Viewable in the in-built CubeIDE terminal
	 *
	 * Shamelessly stolen from the Formula Hybrid team... who probably stole it from someone else.
	 */

  static char buffer[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  int len = strlen(buffer);
  HAL_UART_Transmit(&huart1, (uint8_t *)buffer, len, -1);
  return;
}

static void uart_send(char *buffer){
	return;
}

static void uart_receive(char *buffer){
	return;
}

/*
 * Force Sensor
 */

static int Get_ADC(){
	//TODO: Add Calibration for Max / Min
	// Get ADC value and put into global variable force_resistor

	int adc_value;
	int force_resistor;

	HAL_ADC_Start(&hadc1); // Start polling
	HAL_ADC_PollForConversion(&hadc1, 1); // Check Timeout or switch to IT based
	adc_value = HAL_ADC_GetValue(&hadc1); // Take ADC Value
	HAL_ADC_Stop(&hadc1); // Stop polling

	force_resistor = adc_value; // TODO: Convert from adc_resistor to "resistance" value.
	return force_resistor;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	// TODO: figure out what the hell this does. This was all ripped from deepblueembedded which is probably AI generated.
	return; // ADC Callback for Later Use??
}


/*
 * Inertial Measurement Unit - BNO055
 *
 * BNO055 Notes
 * eul_x: 0->360
 * eul_y: -90->90
 * eul_z: -180->180
 * lin_x: Z-axis
 * lin_y: X-axis
 * lin_z: Y-axis
 *
 * Send Message:
 * Bytes: Start Byte, Read, Reg addr, Length
 *
 * Receive Message:
 * Bytes: ResponseByte, length, Data 1, ..., Data N - (Data 1 at Byte 3, Data N at Byte N+2)
 * REMEMBER: BNO055 is little-endian
 *
 * TODO: Clean up code and remove hard-coded test messages
 */

void bno_enable(){
	/*
	 * Must be called to enable the BNO055 in NDOF mode
	 */
	uint8_t test_message[5];
	test_message[0] = 170;
	test_message[1] = 0x00;
	test_message[2] = 0x3D;
	test_message[3] = 0x01;
	test_message[4] = 0x0C; // xxxx1100b for NDOF selection mode.
	memcpy(uart2_tx_buffer, test_message, 5);
	// Enable BNO055
	HAL_UART_Transmit(&huart2, uart2_tx_buffer, 5, UART_DELAY);
	HAL_UART_Receive(&huart2, uart2_rx_buffer, BUFFER_SIZE, UART_DELAY);
	HAL_UART_Transmit(&huart1, uart2_rx_buffer, BUFFER_SIZE, UART_DELAY);
	return;
}

void bno_write_units(){
	/*
	 * Write a message to the BNO055
	 * TODO: Entire Function (not necessary if we are writing 0x00 since that is reset)
	 */
	uint8_t test_message[64];
	test_message[0] = 170;
	test_message[1] = 0x00;
	test_message[2] = 0x3B;
	test_message[3] = 0x01;
	test_message[4] = 0b00000000;
	// 0x00 = Celsius, Degrees, DPS, m/s^2:
	// 1LSB, 16LSB, 16LSB, 100LSB
	// Pitch:
	memcpy(uart2_tx_buffer, test_message, BUFFER_SIZE);
	print("\r\n");
	HAL_UART_Transmit(&huart2, uart2_tx_buffer, 5, 10);
	HAL_UART_Receive(&huart2, uart2_rx_buffer, BUFFER_SIZE, 10);

	return;
}

uint32_t bno_read(uint8_t address){
	// Read Time @32 Bytes: ~2.5ms
	// Return 2 bytes of data
	uint8_t test_message[BUFFER_SIZE];
	uint16_t combined = 0;
	test_message[0] = 170; // 0xAA: Start Byte
	test_message[1] = 1; // 0x01: Read
	test_message[2] = address; // Register Address
	test_message[3] = 2; // Length
	memcpy(uart2_tx_buffer, test_message, BUFFER_SIZE); // Copy message to tx2 buffer

	HAL_UART_Transmit(&huart2, uart2_tx_buffer, 4, 1);
	HAL_UART_Receive(&huart2, uart2_rx_buffer, BUFFER_SIZE, 5); // Receive Data

	combined = (uart2_rx_buffer[2] << 8 | uart2_rx_buffer[3]);



// 	// Print "combined", which is the MSB and LSB put together to get the full number
//	if (combined != 0){
//		print("%d", combined);
//		print("\r\n");
//	}

//	 // Print Raw Buffer Values
//	 for (int i=0; i<BUFFER_SIZE; i++){
//		 if (uart2_rx_buffer[i] < 0x10){
//			 print("0%x", uart2_rx_buffer[i]);
//		 }
//		 else{
//			 print("%x", uart2_rx_buffer[i]);
//		 }
//	 }
//	 print("\r\n");

	memcpy(uart2_rx_buffer, empty_buffer, BUFFER_SIZE);
	return combined;
}

int bno_read_to_buffer(uint8_t *buffer, uint8_t address, uint8_t length){
	// Read Time @32 Bytes: ~2.5ms
	memcpy(buffer, empty_buffer, BUFFER_SIZE);
	uint8_t test_message[BUFFER_SIZE];
	test_message[0] = 170; // 0xAA: Start Byte
	test_message[1] = 1; // 0x01: Read
	test_message[2] = address; // Register Address
	test_message[3] = length; // Length
	memcpy(uart2_tx_buffer, test_message, BUFFER_SIZE); // Copy message to tx2 buffer

	HAL_UART_Transmit(&huart2, uart2_tx_buffer, 4, 10);
	HAL_UART_Receive(&huart2, buffer, BUFFER_SIZE, 10); // Receive Data
	if (buffer[0] == 0xEE){
		return 0;
	}
	return 1;
}

void print_hex(uint8_t *buffer, uint8_t length){
	if (buffer[0] == 0xEE){
		return; // Error Code
	}
	for (int i=0; i<length; i++){
		if (buffer[i] < 0x10){
			print("0%x", buffer[i]);
		}
		else{
			print("%x", buffer[i]);
		}
	}
}




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
  MX_ADC1_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
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

  int adc_value;
  int state;

  int eul_x = 0;
  int eul_y = 0;
  int eul_z = 0;
  int lin_x = 0;
  int lin_y = 0;
  int lin_z = 0;
  int mag_x = 0;
  int mag_y = 0;
  int mag_z = 0;
  int temp = 0;

  int calibration_status = 0;

  int menu_index = 0;
  int menu_length = 4;
  uint8_t menu_0[32] = "Force Resistor";
  uint8_t menu_1[32] = "IMU Sensor";
  uint8_t menu_2[32] = "GPS Transmit";
  uint8_t menu_3[32] = "GPS Receive";

  char *text_menu =
		  "░████████                         ░██          ░██       ░██ ░██░██        ░██\r\n"
		  "░██    ░██                        ░██          ░██       ░██    ░██        ░██\r\n"
		  "░██    ░██  ░██    ░██  ░███████  ░██    ░██   ░██  ░██  ░██ ░██░██  ░████████\r\n"
		  "░████████   ░██    ░██ ░██    ░██ ░██   ░██    ░██ ░████ ░██ ░██░██ ░██    ░██\r\n";
  char *text_menu_2 =
		  "░██     ░██ ░██    ░██ ░██        ░███████     ░██░██ ░██░██ ░██░██ ░██    ░██\r\n"
		  "░██     ░██ ░██   ░███ ░██    ░██ ░██   ░██    ░████   ░████ ░██░██ ░██   ░███\r\n"
		  "░█████████   ░█████░██  ░███████  ░██    ░██   ░███     ░███ ░██░██  ░█████░██\r\n";


  state = menu;
  print("Entered Main Loop\r\n");

  HAL_Delay(50);
  bno_enable();

  while(1){
	  BSP_LED_Toggle(LED_GREEN);
	  user_button = BSP_PB_GetState(BUTTON_USER);

	  if (state == menu)
	  {
		  // Splash Screen
		  print(text_menu);
		  print(text_menu_2);

		  // Menu Options
		  print("Choose Option:\r\n");
		  print(
				  "1. demo_adc\r\n"
				  "2. demo_bno055\r\n"
				  "3. demo_fullsystem\r\n"
				  "4. demo_maxm10s\r\n"
				  "5. demo_motor\r\n"
				  "6. demo_lock\r\n"
				  "7. demo_stx3\r\n"
				  );
	  }
	  else if (state == demo_adc)
	  {
		  adc_value = Get_ADC();
		  print("Force Sensor: %d\r\n", adc_value);
		  HAL_Delay(100);

	  	  if (user_button == 1){
	  		  state = demo_bno055;
	  		  user_button = 0;
	  		  BSP_LED_Off(LED_GREEN);
	  		  BSP_LED_On(LED_BLUE);
	  		  HAL_Delay(200);
	  		  BSP_LED_Off(LED_BLUE);
	  	  }
	  }
	  else if (state == demo_bno055)
	  {
		  bno_enable();

		  // Data from BNO055 is represented as int16_t. Must be typecast.
		  // Things to watch for: Incoming byte order, data size (1 vs. 2 bytes usually)
		  if (bno_read_to_buffer(storage_buffer, 0x1A, 2) == 1){
			  eul_x = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }
		  if (bno_read_to_buffer(storage_buffer, 0x1C, 2) == 1){
			  eul_y = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }
		  if (bno_read_to_buffer(storage_buffer, 0x1E, 2) == 1){
			  eul_z = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }

		  if (bno_read_to_buffer(storage_buffer, 0x28, 2) == 1){
			  lin_x = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }
		  if (bno_read_to_buffer(storage_buffer, 0x2A, 2) == 1){
			  lin_y = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }
		  if (bno_read_to_buffer(storage_buffer, 0x2C, 2) == 1){
			  lin_z = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }

		  if (bno_read_to_buffer(storage_buffer, 0x34, 1) == 1){
			  temp = (int16_t)(storage_buffer[2]);
		  }

		  if (bno_read_to_buffer(storage_buffer, 0x2A, 2) == 1){
			  mag_x = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }
		  if (bno_read_to_buffer(storage_buffer, 0x28, 2) == 1){
			  mag_y = (int16_t)(storage_buffer[3] << 8 | storage_buffer[2]);
		  }

		  if (bno_read_to_buffer(storage_buffer, 0x34, 1) == 1){
			  mag_z = (int16_t)(storage_buffer[2]);
		  }

		  eul_x = eul_x / 16;
		  eul_y = eul_y / 16;
		  eul_z = eul_z / 16;
		  lin_x = lin_x / 1;
		  lin_y = lin_y / 1;
		  lin_z = lin_z / 1;
		  temp = temp * 1;

		  // TODO: Figure out how to print floats.
		  // print("lX: %.1f, lY: %.1f, lZ: %.1f, eX: %.1f, eY: %.1f, eZ: %.1f, Temp: %.1f\r\n", lin_x, lin_y, lin_z, eul_x, eul_y, eul_z, temp);
		  print("lX: %d, lY: %d, lZ: %d, eX: %d, eY: %d, eZ: %d, Temp: %d\r\n", lin_y, lin_x, lin_z, eul_x, eul_y, eul_z, temp); // NOTE: LINEAR VALUES XYZ ARE NOT CORRESPONDING TO WHAT IS PRINTED!!!

	  	  if (user_button == 1){
	  		  state = demo_adc;
	  		  user_button = 0;
	  		  BSP_LED_Off(LED_GREEN);
	  		  BSP_LED_On(LED_BLUE);
	  		  HAL_Delay(200);
	  		  BSP_LED_Off(LED_BLUE);
	  	  }

	  }
	  else{
		  BSP_LED_Toggle(LED_RED);
		  print("Error: Please Restart Demo.");
	  }
  }
  }




















    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

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
	  print("YOU DIED\n");
	  print("Press any key to respawn.\n");
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
