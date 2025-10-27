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

typedef enum{
	LOCK_ENABLED = 0,
	LOCK_DISABLED = 1,
} LockMode;

void Motor_Initialize(void);
void Motor_SetSpeed(float duty_cycle);
void Motor_SetDirection(MotorDirection direction);
void Lock_Initialize();
void Lock_SetStatus(LockMode mode);

#endif /* INC_MOTOR_CONTROL_H_ */
