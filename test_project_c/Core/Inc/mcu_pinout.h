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

// E2: CN9-14
#define MOTORDRIVER_AI1_PORT GPIOE
#define MOTORDRIVER_AI1_PIN GPIO_PIN_2
// E4: CN9-16
#define MOTORDRIVER_AI2_PORT GPIOE
#define MOTORDRIVER_AI2_PIN GPIO_PIN_4
// A5: CN7-10
#define MOTORDRIVER_PWMA_PORT GPIOA
#define MOTORDRIVER_PWMA_PIN GPIO_PIN_5

// Solenoid Lock
// Outputs: BO1 (only one pin needed)
// You MUST ground BI2

// E5: CN9-18
#define LOCK_BI1_PORT GPIOE
#define LOCK_BI1_PIN GPIO_PIN_5

// E6: CN9-20
#define LOCK_PWMB_PORT GPIOE
#define LOCK_PWMB_PIN GPIO_PIN_6

#endif /* INC_MCU_PINOUT_H_ */
