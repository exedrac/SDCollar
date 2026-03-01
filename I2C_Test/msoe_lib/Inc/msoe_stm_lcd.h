/*
 * msoe_stm_lcd.h
 *
 *  Created on: Jun 9, 2023
 *      Author: widder
 *
 *        Change log:
 *        Date		Eng.	Description
 *        8/12/24	krw		Added drawing functions - pixel, lines, etc.
  */

//  Source files for interfacing the STM32U575 to the Adafruit
//  1.44" 128 x 128 color TFT LCD using the ST7735 chip.
//  With the 12-font, it has 10 rows and 18 columns of characters.
//  The numbering starts at the top-left corner (0, 0)
//  Uses functions from STM and Adafruit
//
//  Initialize the LCD before use via the function
//		LCD_IO_Init().
//  Other functions available are:
//    LCD_clear()		clears the screen
//    LCD_print_char(uint8_t row, uint8_t col, uint8_t Ascii)		write a character
//    LCD_print_str(uint8_t row, uint8_t col, unsigned char *pStr)			write a string
//    LCD_print_bin8(uint8_t row, uint8_t col, uint8_t val)			8-bit number in binary
//    LCD_print_bin16(uint8_t row, uint8_t col, uint16_t val)		16-bit number in binary
//    LCD_print_hex8(uint8_t row, uint8_t col, uint8_t val)			8-bit number in hexadecimal (hex)
//    LCD_print_hex16(uint8_t row, uint8_t col, uint16_t val)		16-bit number in hex
//    LCD_print_hex32(uint8_t row, uint8_t col, uint32_t val)		32-bit number in hex
//    LCD_print_udec3(uint8_t row, uint8_t col, uint8_t val)		8-bit unsigned int (3 digits)
//    LCD_print_udec5(uint8_t row, uint8_t col, uint16_t val)
//    LCD_print_udec10(uint8_t row, uint8_t col, uint32_t val)
//    LCD_print_dec3(uint8_t row, uint8_t col, int8_t val)			8-bit signed int (3 digits)
//    LCD_print_dec5(uint8_t row, uint8_t col, int16_t val)
//    LCD_print_dec10(uint8_t row, uint8_t col, int32_t val)
//    LCD_print_float(uint8_t row, uint8_t col, float val)			floating point value
//	  LCD_print_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code)
//	  LCD_print_HLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
//	  LCD_print_VLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
//	  LCD_print_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
//	  LCD_print_Rect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
//	  LCD_print_Circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
//	  LCD_print_Ellipse(int Xpos, int Ypos, int XRadius, int YRadius)
//	  LCD_print_Bitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp)
//
//
//
////////////////////////////////////////////////////////////////
//
// Connection name / Adafruit LCD Module / STM32U575 Hardware Configuration (pinout)
//
// Vin						pin 1		3.3V	CN8-7
// 3v3 out					pin 2		N/C
// GND        				pin 3		GND		CN10-22
// SCK 					   	pin 4		PE13	CN10-10
// SO         				pin 5		PE14	CN10-28
// SI						pin 6		PE15	CN10-30
// TCS			   			pin 7 		PE12	CN10-23
// RST       				pin 8		PE9		CN10-4		Active LOW
// D/C (data/commandBar)	pin 9		PE10	CN10-24		command LOW
// CCS       				pin 10		PE8		CN10-18		Active LOW
// Lite			    		pin 11 		N/C
//
////////////////////////////////////////////////////////////////


#ifndef MSOE_STM_LCD_H_
#define MSOE_STM_LCD_H_

#include "stm32u575xx.h"
#include "stdint.h"
#include "st7735.h"
#include "stm32_adafruit_lcd.h"

/********************************* LINK LCD ***********************************/
/**
  * @brief  Initializes the LCD
  * @retval None
  */
void LCD_GPIO_Config(void);

/********************************* LINK LCD ***********************************/
/**
  * @brief  Initializes the LCD
  * @retval None
  */
void LCD_SPI_Config(void);

/********************************* LINK LCD ***********************************/
/**
  * @brief  Initializes the LCD
  * @retval None
  */
void LCD_IO_Init(void);

/**
  * @brief  Writes a command to the LCD .
  * @param  LCDReg Address of the selected register.
  * @retval None
  */
void LCD_IO_WriteReg(uint8_t LCDReg);

/**
  * @brief  Writes data to select the LCD register.
  *         This function must be used after st7735_WriteReg() function
  * @param  Data data to write to the selected register.
  * @retval None
  */
void LCD_IO_WriteData(uint8_t Data);

/**
* @brief  Write register value.
* @param  pData Pointer on the register value
* @param  Size Size of byte to transmit to the register
* @retval None
*/
void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size);

/**
* @brief  Clear LCD screen
* @param  None
* @retval None
*/
void LCD_clear(void);

/**
* @brief  print a character to the LCD.
* @param  row Row to print in
* @param  col Column to start in
* @param  Ascii Character (in ASCII) to print
* @retval None
*/
void LCD_print_char(uint8_t row, uint8_t col, uint8_t Ascii);

/**
* @brief  print a string to the LCD.
* @param  row Row to print in
* @param  col Column to start in
* @param  Pointer to string data to print
* @retval None
*/
void LCD_print_str(uint8_t row, uint8_t col, unsigned char *pStr);

/**
* @brief  Helper function to avoid the use of the math library
*    Calculate POSITIVE powers of 10
* @param  exp exponent
* @retval pow
*/

///////////   Number display routines   ///////////////////////////
//
/**
* @brief  Display binary
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_bin8(uint8_t row, uint8_t col, uint8_t val);

void LCD_print_bin16(uint8_t row, uint8_t col, uint16_t val);

/**
* @brief  Display 8 bit Hex
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*///

void LCD_print_hex8(uint8_t row, uint8_t col, uint8_t val);

/**
* @brief  Display  16bit Hex
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*///

void LCD_print_hex16(uint8_t row, uint8_t col, uint16_t val);


/**
* @brief  Display 32 bit Hex
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*///

void LCD_print_hex32(uint8_t row, uint8_t col, uint32_t val);


/**
* @brief  Helper function to avoid the use of the math library
*    Calculate POSITIVE powers of 10
* @param  exp exponent
* @retval pow
*/


int pwr10(uint8_t exp);

/**
* @brief  Display 3 digit unsigned Decimal
*    supports 8 bit unsigned values
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_udec3(uint8_t row, uint8_t col, uint8_t val);


/**
* @brief  Display 5 digit unsigned Decimal
*    supports 16 bit unsigned values
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_udec5(uint8_t row, uint8_t col, uint16_t val);


/**
* @brief  Display 10 digit unsigned Decimal
*    supports 32 bit unsigned values
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_udec10(uint8_t row, uint8_t col, uint32_t val);


/**
* @brief  Display 3 digit signed Decimal
*    supports 8 bit signed values
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_dec3(uint8_t row, uint8_t col, int8_t val);


/**
* @brief  Display 5 digit signed Decimal
*    supports 16 bit signed values
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_dec5(uint8_t row, uint8_t col, int16_t val);


/**
* @brief  Display 10 digit signed Decimal
*    supports 32 bit signed values
* @param  row Row to print in
* @param  col Column to start in
* @param  val value to print
* @retval None
*/

void LCD_print_dec10(uint8_t row, uint8_t col, int32_t val);

/**
* @brief  Helper function to avoid the use of the math library
*    Calculate powers of 10 as a float
* @param  exp exponent
* @retval pow
*/

float pwr10f(int8_t exp);

	/**
	* @brief  Display float in scientific notation
	*    x.xxxxExx
	* @param  row Row to print in
	* @param  col Column to start in
	* @param  val value to print
	* @retval None
	*/


void LCD_print_float(uint8_t row, uint8_t col, float val);

/**
  * @brief  Wait for loop in ms.
  * @param  Delay in ms.
  * @retval None
  */
void LCD_Delay(uint32_t Delay);


/**
  * @brief  dras a single pixel with the given color
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  RGB_Code: Pixel color in RGB mode (5-6-5)
  * @retval None
*/

void LCD_print_pixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);

/**
  * @brief  Draws a horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void LCD_print_HLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);

/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void LCD_print_VLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);

/**
  * @brief  Draws a line (between two points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @retval None
  */
void LCD_print_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void LCD_print_Rect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**
  * @brief  Draws a circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void LCD_print_Circle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void LCD_print_Ellipse(int Xpos, int Ypos, int XRadius, int YRadius);
/**
  * @brief  Draws a bitmap picture loaded in the STM32 MCU internal memory.
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pBmp: Pointer to Bmp picture address
  * @retval None
  */
void LCD_print_Bitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp);



#endif /* MSOE_STM_LCD_H_ */
