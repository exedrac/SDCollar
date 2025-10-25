/*
 * motor_control.c
 *
 *  Created on: Oct 24, 2025
 *      Author: eickhoffp
 */

#include "motor_control.h"

void Motor_Init(void){
	//TODO: I don't think there's much to initialize though...
}
void Motor_SetSpeed(float duty_cycle){
	//TODO: Verify at 0 and 100 edge cases
	extern TIM_HandleTypeDef htim2;

	uint32_t max_count = htim2.Init.Period;
	uint32_t pulse_width = (uint32_t)(duty_cycle * max_count);
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
}
