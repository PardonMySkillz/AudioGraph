/*
 * lcd.h
 *
 *  Created on: Nov 2, 2025
 *      Author: hcwon
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f4xx.h"


#define LCD_REG   (*((volatile uint16_t *) 0x60000000))
#define LCD_DATA  (*((volatile uint16_t *) 0x60080000)) // Data register (A18 = 1)

#define LCD_BL_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_1
#define LCD_RESET_GPIO_Port GPIOA
#define LCD_RESET_Pin GPIO_PIN_8

#define LCD_WIDTH 240
#define LCD_HEIGHT 320

#define CHAR_WIDTH   8
#define CHAR_HEIGHT  16

#define LCD_COLOR_BLACK       0x0000
#define LCD_COLOR_WHITE       0xFFFF
#define LCD_COLOR_RED         0xF800
#define LCD_COLOR_GREEN       0x07E0
#define LCD_COLOR_BLUE        0x001F
#define LCD_COLOR_YELLOW      0xFFE0
#define LCD_COLOR_CYAN        0x07FF
#define LCD_COLOR_MAGENTA     0xF81F

// Function prototypes
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint16_t data);
void LCD_WriteData8(uint8_t data);
void LCD_WriteMultipleData(uint16_t *data, uint16_t count);
void LCD_Reset(void);
void LCD_SetBacklight(uint8_t state);
void LCD_Init(void);
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_Fill(uint16_t color);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_DrawHLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void LCD_DrawVLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
void LCD_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color);
void LCD_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

#endif /* INC_LCD_H_ */
