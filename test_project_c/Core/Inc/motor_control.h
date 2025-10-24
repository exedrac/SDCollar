/*
 * motor_control.h
 *
 *  Created on: Oct 24, 2025
 *      Author: eickhoffp
 */

#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

#include "mcu_pinout.h"
#include "stm32u5xx_hal.h"

typedef enum{
	MOTOR_STOP = 0,
	MOTOR_CCW = 1,
	MOTOR_CW = 2
} MotorDirection;

void Motor_Init(void);
void Motor_SetSpeed(uint8_t duty_cycle);
void Motor_SetDirection(MotorDirection direction);

#endif /* INC_MOTOR_CONTROL_H_ */
