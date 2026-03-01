/*
 * msoe_stm_delay.c
 *
 *  Created on: Jun 26, 2023
 *      Author: widder
 *
 *      MSOE library files to implement microsecond and millisecond
 *        delays
 *
 *
 *        Change log:
 *        Date		Eng.	Description
 *        1/19/24	krw		Corrected improper handling of delay_ms with
 *        						value less than 2 which caused micro to hang
 *        3/2/24	krw		Corrected us bug - cleared bits 11:9 in CFGR
 *        						before setting to 0b010
 */

// These functions are used to get delays in either microseconds or milliseconds
//    They assume that the MSIK clock (also used for LCD SPI interface) is
//    left at its default condition of enabled and running at 4 MHz.
//  In the main code, it is necessary to include two statements for clock system
//    setup
//   	RCC->APB3ENR |= (1 << 13); // enable LPTIM4 clock
//   	//RCC->CCIPR3 &= ~(3 << 20);  // select MSIK for SPI1 kernel clock (4 MHz)
//         // this is the default value for these bits, so no action required


#include <msoe_stm_delay.h>


/********************************* Delays ***********************************/
/**
 * @brief  Initialize infrastructure for delay
 * @param  None
 * @retval None
 *
 */
void msoe_delay_init(void)
{
	// enable MSIK clock to use for LPTIM4 at 4 MHz
	RCC->CR |= (1 << 4);  // enable MSIK clock
	RCC->APB3ENR |= (1 << 13);   // enable LPTIM4 clock for delays
	RCC->CCIPR3 &= ~(3 << 8);  // select MSIK for LPTIM4 kernel clock (4 MHz)
}

/**
 * @brief  Implement a microsecond delay
 * @param  num_us number of usec to delay
 * @retval None
 *
 * NOTE: the minimum value is 12 usec. The maximum is 65535.
 */
void msoe_delay_us(uint16_t num_us)
{
	if(num_us > 11)
	{
		// adjust value to account for setup code execution time
		num_us -= 10;
		// make sure timer is disabled to be able to set prescale
		LPTIM4->CR &= ~(1 << 0);
		// set the clock prescale to 4 to give a 1 usec tick
		// assuming clock source is MSIK at 4 MHz
		LPTIM4->CFGR &= ~(0b111 << 9);
		LPTIM4->CFGR |= (0b010 << 9);
		// enable timer so rest of the registers can be modified
		LPTIM4->CR |= (1 << 0);
		//reset counter and wait to complete action
		while(LPTIM4->CR & (1 << 3));
		LPTIM4->CR |= (1 << 3);
		while(LPTIM4->CR & (1 << 3));
		// clear repitition register - only need one timer cycle
		LPTIM4->RCR = 0;
		// load delay value into autoreload register
		LPTIM4->ARR = (uint32_t)num_us - 1;
		// make sure both writes are complete
		while((LPTIM4->ISR & ((1 << 8) | (1 << 4))) < ((1 << 8) | (1 << 4)));
		// clear both flags and UE flag to be safe
		LPTIM4->ICR |= ((1 << 8) | (1 << 7) | (1 << 4));
		// start timer (SNGSTRT bit) and wait for ISR:UE to be set indicating done
		LPTIM4->CR |= (1 << 1);
		//	LPTIM4->CR |= (1 << 2);
		while(!(LPTIM4->ISR & (1 << 7)));
		// clear UE bit and return
		LPTIM4->ICR |= (1 << 7);
		// make sure timer is disabled
		LPTIM4->CR &= ~(1 << 0);
	}

	return;
}

/**
 * @brief  Implement a millisecond delay
 * @param  num_ms number of msec to delay
 * @retval None
 *
 * NOTE: the maximum delay is 65535 ms
 */
void msoe_delay_ms(uint16_t num_ms)
{
	if(num_ms < 2)
		msoe_delay_us(1000);
	else
	{
		// make sure timer is disabled to be able to set prescale
		LPTIM4->CR &= ~(1 << 0);
		// set the clock prescale to 128 to give a 32 usec tick
		// assuming clock source is MSIK at 4 MHz
		LPTIM4->CFGR |= (0b111 << 9);
		// enable timer so rest of the registers can be modified
		LPTIM4->CR |= (1 << 0);
		//reset counter and wait to complete action
		while(LPTIM4->CR & (1 << 3));
		LPTIM4->CR |= (1 << 3);
		while(LPTIM4->CR & (1 << 3));
		// set repetition register to 30 - only need 31 timer cycles
		LPTIM4->RCR = 30;
		// load delay value into autoreload register
		LPTIM4->ARR = (uint32_t)num_ms - 1;
		// make sure both writes are complete
		while((LPTIM4->ISR & ((1 << 8) | (1 << 4))) < ((1 << 8) | (1 << 4)));
		// clear both flags and UE flag to be safe
		LPTIM4->ICR |= ((1 << 8) | (1 << 7) | (1 << 4));
		// start timer (SNGSTRT bit) and wait for ISR:UE to be set indicating done
		LPTIM4->CR |= (1 << 1);
		//	LPTIM4->CR |= (1 << 2);
		while(!(LPTIM4->ISR & (1 << 7)));
		// clear UE bit
		LPTIM4->ICR |= (1 << 7);
		//	// set repetition register to 0 - only need one timer cycle
		//	LPTIM4->RCR = 0;
		//	// load delay value into autoreload register to give 1/4 of original counts
		//	LPTIM4->ARR = ((uint32_t)num_ms >> 2) ;
		//	// make sure both writes are complete
		//	while((LPTIM4->ISR & ((1 << 8) | (1 << 4))) < ((1 << 8) | (1 << 4)));
		//	// clear both flags and UE flag to be safe
		//	LPTIM4->ICR |= ((1 << 8) | (1 << 7) | (1 << 4));
		//	// start timer (SNGSTRT bit) and wait for ISR:UE to be set indicating done
		//	LPTIM4->CR |= (1 << 1);
		////	LPTIM4->CR |= (1 << 2);
		//	while(!(LPTIM4->ISR & (1 << 7)));
		//	// clear UE bit
		//	LPTIM4->ICR |= (1 << 7);
		// make sure timer is disabled
		LPTIM4->CR &= ~(1 << 0);
	}
	return;
}


