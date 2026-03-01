/*
 * msoe_clk_setup.c
 *
 *  Created on: Jun 6, 2023
 *      Author: johnsontimoj
 *
 *
 *  Change log:
 *  	Date		Author		Description
 *  	12/26/23	KWidder		Added Vcore power changes to allow higher clock frequencies
 *  								changes to msoe_clk_setup
 */
////////////////////////////////////////////////////////
//
// default setup (spec 11.4)
// 		source MSI
// 		frequency is 4MHZ (MSIRC1)
//		feeding MSIS and MSIK
//		using MSIS as sysclk
//
//      using HSI for USB ????
//
///////////////////////////////////////////////////////
//
// Plan:
// 		use 4MHz source but run it through PLL1 to change frequency
//		continue with MSI as clk source
//		feed MSI to PLL1 to change frequencies
//		select PLL1 R output for sysclk
//
//		include code to feed sysclk out to a pin PA8
//
//////////////////////////////////////////////////////
#include "stm32u5xx.h"

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
int msoe_clk_setup(uint8_t target_clk){
	////////////////////////
	// USE default clock to setup PLL
	//
	// Need a clock operating to do programming
	/////////////////////////

	///////////////////////////////////
	// set max clock
	//
	if(target_clk > 160)
		target_clk = 160;

//	///////////////////////////////////
//	// enable HSI and HSI48 for USB ???	(spec 11.8.1)
//	// 		HSI ON (bit 8), HSI rdy (bit 10)
//	// 		HSI48 ON (bit 12), HSI48 rdy (bit 13)
//	RCC->CR |= 0x00000100;
//	while(!(RCC->CR & 0x00000400)){
//	}
//	RCC->CR |= 0x00001000;
//	while(!(RCC->CR & 0x00002000)){
//	}

	// **********************************************
	// setup Vcore range to allow for higher frequencies
	//      12/26/23
	//
	// enable clock for PWR module
	RCC->AHB3ENR |= RCC_AHB3ENR_PWREN;
	// RCC->PLL1CFGR &= ~0x0F;  // bypass (/1)  These bits are 0 by default
	RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1SRC_0; // set SRC bits to 0b01 for MSIS
	// select voltage range 1 to allow max frequency
	PWR->VOSR |= PWR_VOSR_VOS;  // Ob11
	// wait for VOSRDY
	while(!(PWR->VOSR & PWR_VOSR_VOSRDY))
	{}
	// enable EPOD booster
	PWR->VOSR |= PWR_VOSR_BOOSTEN;  // Ob11
	// wait for EPOD booster
	while(!(PWR->VOSR & PWR_VOSR_BOOSTRDY))
	{}
	// handle wait state setting for Flash and SRAM
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;  // 4 W.S. needed for 160 MHz
	//RAMCFG->M1CR &= ~(0b111 << 16); // 0 W.S. needed for SRAM up to 160 MHz (0 is default for all 5)

	//  **********************************************


	//////////////////////////////////////////////
	// setup PLL1 to drive the system clock
	//
	// 	src is MSIS
	//////////////////////////////////////////////
	//
	// Setup PLL	(spec 11.8.9)
	// 		src (bit 1:0) - msis (1), fracken??ON (bit 4), R output en (bit 18),
	//					0x40011
	RCC->PLL1CFGR = 0x40011;
	//
	// Setup PLL dividers	(spec 11.8.12)
	//		p, q, r all set to default -> 1/2, n set to default 2 --> 4MHz default clock
	//			p (bits15:9) - 1, q (bits22:16) - 1, r (bits30:24) - 1, n (bits8:0) - 1
	RCC->PLL1DIVR = 0x1010201;
	//
	// Modify clock frequency based on input
	//		n is the feedback divider (Multiplier)
	//		multiplier N set to 2x desired mult since r is set to divide by 2 (default)
	//			code for n (bits8:0) - (desired value - 1)
	//			e.g. desire 4MHz 	--> multiplier = 4MHz/4MHz = 1
	//								--> N = 2
	//								--> n = 1
	//			e.g. desire 160MHz 	--> multiplier = 160MHz/4MHz = 40
	//								--> N = 80
	//								--> n = 79
	RCC->PLL1DIVR &= ~0x1FF;
	RCC->PLL1DIVR |= ((2 * (target_clk / 4)) - 1);
//		0x100004F for 80/2 40*4 = 160MHz
//		0x1000000 for 2/2  1*4 = 4MHz
//	RCC->PLL1DIVR = 0x1010201;		// using 4MHz for now, 01
//	RCC->PLL1DIVR = 0x101020F;		// using 32MHz for now, 15
//	RCC->PLL1DIVR = 0x1010227;		// using 80MHz for now, 39
	//
	// Turn on PLL and wait for ready	(spec 11.8.1)
	// 		PLL1 on (bit 24), PLL1 rdy (bit 25)
	RCC->CR |= 0x01000000;
	while(!(RCC->CR & 0x02000000)){
	}
	//
	// Switch sysclk to PLL and wait for ready	(spec 11.8.6)
	//		sel pll (bits 1:0) 1 1, pll rdy (bits 3:2) = 1 1
	RCC->CFGR1 |= 0x3;
	while((RCC->CFGR1 & 0x0000000C) != 0x0C){
	}

	return 0;
}// end msoe_clk_setup

//////////////////////////////////////////////
//
// use MCO to view clock
//
//		dividing by 16 to view higher clock frequencies with the analog discovery
//
// 		MCO is on PA8	(datasheet table 26)
// 		Alternate function AF0 (datasheet table 27)
////////////////////
void msoe_clk_to_mco(void){
	// Enable GPIOA clock	(spec 11.8.30)
	// 		bit 0 in RCC->AHB2ENR1
	RCC->AHB2ENR1 |= 0x0001;
	//
	// Set MCO to Sysclk	(spec 11.8.6)
	// 		divide by 16 (bits 30:28) 1 0 0
	// 		use sysclk (bits 27:24) 0 0 0 1
	RCC->CFGR1 &= ~0x7F000000;
	RCC->CFGR1 |= 0x41000000;
	//
	// set PA8 to alternate function MC0	(spec 13.4.10)
	//		Pins 15:8 start in AFR[1]
	// 		Bits 3:0 set to AF0 (0 0 0 0)
	GPIOA->AFR[1] &= ~0x0000000F;
	//	GPIOA->AFR[1] |= 0x00000000;
	//
	// set PA8 to alt fn mode	(spec 13.4.1)
	// 		bits 17:16 to 1 0
	GPIOA->MODER &= ~0x00030000;
	GPIOA->MODER |= 0x00020000;
	//
	// set PA8 to high speed	(spec 13.4.3)
	// 		bits 17:16 to 1 0
	GPIOA->OSPEEDR &= ~0x00030000;
	GPIOA->OSPEEDR |= 0x00020000;

	return;
}// end msoe_clk_to_mco


