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
void Motor_SetSpeed(uint8_t duty_cycle){

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
