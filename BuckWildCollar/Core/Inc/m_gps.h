/*
 * m_gps.h
 *
 *  Created on: May 1, 2026
 *      Author: eickhoffp
 */

#ifndef INC_M_GPS_H_
#define INC_M_GPS_H_

#include <stdint.h>
#include "stm32u5xx_hal.h"   // for UART_HandleTypeDef

#ifdef __cplusplus
extern "C" {
#endif

// Public API
void gps_acquire(char *buffer, int size);

// You typically do NOT expose this, but since HAL needs it globally
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* INC_M_GPS_H_ */
