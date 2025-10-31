/*
 * mcu_pinout.h
 *
 *  Created on: Oct 24, 2025
 *      Author: eickhoffp
 */

#ifndef INC_MCU_PINOUT_H_
#define INC_MCU_PINOUT_H_

// Motor Driver
// Outputs: AO1, AO2

// PE2: CN9-14
#define MOTORDRIVER_AI1_PORT GPIOE
#define MOTORDRIVER_AI1_PIN GPIO_PIN_2
// PE4: CN9-16
#define MOTORDRIVER_AI2_PORT GPIOE
#define MOTORDRIVER_AI2_PIN GPIO_PIN_4
// PA5: CN7-10
#define MOTORDRIVER_PWMA_PORT GPIOA
#define MOTORDRIVER_PWMA_PIN GPIO_PIN_5

// Solenoid Lock
// Outputs: BO1 (only one pin needed)
// You MUST ground BI2

// PE5: CN9-18
#define LOCK_BI1_PORT GPIOE
#define LOCK_BI1_PIN GPIO_PIN_5

// PE6: CN9-20
#define LOCK_PWMB_PORT GPIOE
#define LOCK_PWMB_PIN GPIO_PIN_6

#endif /* INC_MCU_PINOUT_H_ */

// I2C Bus
// PB8: CN7-2: I2C1_SCL
#define I2C1_SCL_PORT GPIOB
#define I2C1_SCL_PIN GPIO_PIN_8

// PB9: CN7-4: I2C1_SDA
#define I2C1_SDA_PORT GPIOB
#define I2C1_SDA_PIN GPIO_PIN_9

// UART
// PG7: CN10-14: USART_A_TX
#define USART_A_TX_PORT GPIOG
#define USART_A_TX_PIN GPIO_PIN_7

// PG8: CN10-16: USART_A_RX
#define USART_A_RX_PORT GPIOG
#define USART_A_RX_PIN GPIO_PIN_8

