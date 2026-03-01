/*
 * msoe_stm_delay.h
 *
 *  Created on: Jun 26, 2023
 *      Author: widder
 *
 *      This file contains functions to implement delays.
 *      Before using them in your code, you must first call
 *      	msoe_delay_init() to take care of some setup
 *      Functions available:
 *      	msoe_delay_us(uint16_t num_us)  NOTE: the minimum value is 12 usec. The maximum is 65535.
 *      	msoe_delay_ms(uint16_t num_ms)  NOTE: the maximum delay is 65535 ms
 */

#ifndef MSOE_STM_DELAY_H_
#define MSOE_STM_DELAY_H_

#include "stm32u575xx.h"
#include "stdint.h"

/********************************* Delays ***********************************/
/**
  * @brief  Initialize infrastructure for delay
  * @param  None
  * @retval None
  *
  */
void msoe_delay_init(void);

/**
  * @brief  Implement a microsecond delay
  * @param  num_us number of usec to delay
  * @retval None
  *
  * NOTE: the minimum value is 12 usec. The maximum is 65535.
  */
void msoe_delay_us(uint16_t num_us);

/**
  * @brief  Implement a millisecond delay
  * @param  num_ms number of msec to delay
  * @retval None
  *
  * NOTE: the maximum delay is 65535 ms
  */
void msoe_delay_ms(uint16_t num_ms);


#endif /* MSOE_STM_DELAY_H_ */
