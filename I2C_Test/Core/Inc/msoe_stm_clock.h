/*
 * msoe_stm_clock.h
 *
 *  Created on: Jun 27, 2023
 *      Author: widder
 *
 *
 *  Change log:
 *  	Date		Author		Description
 *  	1/14/24	KWidder		Added msoe_clk_setup() and msoe_clk_to_mco() prototypes
 *  								(files from Dr. Tim Johnson)
 *  								removed msoe_sysclk_160MHz() prototype
 *
 */

#ifndef MSOE_STM_CLOCK_H_
#define MSOE_STM_CLOCK_H_

#include "stm32u575xx.h"
#include "stdint.h"

/********************************* Clock functions ***********************************/
/**
  * @brief  Set system clock (SYSCLK) to 160 MHz
  * @param  None
  * @retval None
  */

//void msoe_sysclk_160MHz(void);
///////////////////////////////////////////////////
//
// msoe_clk_setup()
//
// Setup the STM32_U575iq clock frequency
//
// input - desired clock frequency in MHz
//
// MUST BE A WHOLE MULTIPLE OF 4MHz in this simple implementation
// MAX input is 160MHz
//
// ******************************************************************
//		12/26/23	KWidder		Added Vcore power changes to allow higher clock frequencies
//  		Default is range 4. The frequency limits for each range are as follows:
//				Range 1:	160 MHz
//				Range 2:	110 MHz
//				Range 3:	55 MHz
//				Range 4:	25 MHz
//
//		Code was added to change the power range to range 1 to allow for any frequency. If power
//			consumption is a concern, it will need further changes to tailor the range to the
//			desired frequency.
//
//  *****************************************************************
//
//////////////////////////////////////////////////
int msoe_clk_setup(uint8_t target_clk);


//////////////////////////////////////////////
//
// use MCO to view clock
//
//		dividing by 16 to view higher clock frequencies with the analog discovery
//
// 		MCO is on PA8	(datasheet table 26)
// 		Alternate function AF0 (datasheet table 27)
////////////////////


void msoe_clk_to_mco(void);

#endif /* MSOE_STM_CLOCK_H_ */
