/*
 * motor_control.c
 *
 *  Created on: Oct 24, 2025
 *      Author: eickhoffp
 *
 *      Motor Driver for DC Motor and Solenoid Lock
 */

#include "motor_control.h"
#include "mcu_pinout.h"

void Motor_Initialize(void){
	// Stop Motor and Set Speed to 0
	extern TIM_HandleTypeDef htim2;
	HAL_GPIO_WritePin(MOTORDRIVER_AI1_PORT, MOTORDRIVER_AI1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MOTORDRIVER_AI2_PORT, MOTORDRIVER_AI2_PIN, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
}
void Motor_SetSpeed(float duty_cycle){
	extern TIM_HandleTypeDef htim2;
	int pulse_width;
	int max_count = htim2.Init.Period;

	// Bound duty cycle between 0 and 1
	if (duty_cycle < 0){
		pulse_width = 0;
	}
	else if(duty_cycle > 1){
		pulse_width = max_count;
	}
	else{
		pulse_width = (uint32_t)(duty_cycle * max_count);
	}
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse_width);
}
void Motor_SetDirection(MotorDirection direction){
	if (direction == MOTOR_STOP){
		HAL_GPIO_WritePin(MOTORDRIVER_AI1_PORT, MOTORDRIVER_AI1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOTORDRIVER_AI2_PORT, MOTORDRIVER_AI2_PIN, GPIO_PIN_RESET);
	}
	else if (direction == MOTOR_CCW){
		HAL_GPIO_WritePin(MOTORDRIVER_AI1_PORT, MOTORDRIVER_AI1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOTORDRIVER_AI2_PORT, MOTORDRIVER_AI2_PIN, GPIO_PIN_SET);
	}
	else if (direction == MOTOR_CW){
		HAL_GPIO_WritePin(MOTORDRIVER_AI1_PORT, MOTORDRIVER_AI1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(MOTORDRIVER_AI2_PORT, MOTORDRIVER_AI2_PIN, GPIO_PIN_RESET);
	}
	else{
		// Error -> Stop Motor
		HAL_GPIO_WritePin(MOTORDRIVER_AI1_PORT, MOTORDRIVER_AI1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(MOTORDRIVER_AI2_PORT, MOTORDRIVER_AI2_PIN, GPIO_PIN_RESET);

		// TODO: What does Error Handler Do?
		// Error_Handler();
	}
	return;
}
void Lock_Initialize(){
	HAL_GPIO_WritePin(LOCK_BI1_PORT, LOCK_BI1_PIN, GPIO_PIN_RESET);
}
void Lock_SetStatus(LockMode mode){
	if (mode == LOCK_DISABLED){
		HAL_GPIO_WritePin(LOCK_PWMB_PORT, LOCK_PWMB_PIN, GPIO_PIN_SET);
	}
	else if (mode == LOCK_ENABLED){
		HAL_GPIO_WritePin(LOCK_PWMB_PORT, LOCK_PWMB_PIN, GPIO_PIN_RESET);
	}
	else{
		HAL_GPIO_WritePin(LOCK_PWMB_PORT, LOCK_PWMB_PIN, GPIO_PIN_RESET);
	}
}
