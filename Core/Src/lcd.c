/*
 * lcd.c
 *
 *  Created on: Nov 2, 2025
 *      Author: hcwon
 */
#include "lcd.h"
#include "stdlib.h"
#include "ascii.h"


void LCD_WriteCommand(uint8_t cmd)
{
    LCD_REG = cmd;
}

void LCD_WriteData(uint16_t data)
{
    LCD_DATA = data;
}

void LCD_WriteData8(uint8_t data)
{
    LCD_DATA = data;
}

void LCD_WriteMultipleData(uint16_t *data, uint16_t count)
{
    for(uint16_t i = 0; i < count; i++)
    {
        LCD_DATA = data[i];
    }
}

void LCD_Reset(void)
{
    // Hardware reset
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
}

void LCD_SetBacklight(uint8_t state)
{
    if (state)
        HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
}


void LCD_Init(void)
{
    // Reset the LCD
    LCD_Reset();

    // Init sequence for ILI9341
    LCD_WriteCommand(0xCF);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x83);
    LCD_WriteData8(0X30);

    LCD_WriteCommand(0xED);
    LCD_WriteData8(0x64);
    LCD_WriteData8(0x03);
    LCD_WriteData8(0X12);
    LCD_WriteData8(0X81);

    LCD_WriteCommand(0xE8);
    LCD_WriteData8(0x85);
    LCD_WriteData8(0x01);
    LCD_WriteData8(0x79);

    LCD_WriteCommand(0xCB);
    LCD_WriteData8(0x39);
    LCD_WriteData8(0x2C);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x34);
    LCD_WriteData8(0x02);

    LCD_WriteCommand(0xF7);
    LCD_WriteData8(0x20);

    LCD_WriteCommand(0xEA);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x00);

    LCD_WriteCommand(0xC0);    // Power control
    LCD_WriteData8(0x26);      // VRH[5:0]

    LCD_WriteCommand(0xC1);    // Power control
    LCD_WriteData8(0x11);      // SAP[2:0];BT[3:0]

    LCD_WriteCommand(0xC5);    // VCM control
    LCD_WriteData8(0x35);
    LCD_WriteData8(0x3E);

    LCD_WriteCommand(0xC7);    // VCM control2
    LCD_WriteData8(0xBE);

    LCD_WriteCommand(0x36);    // Memory Access Control
    LCD_WriteData8(0x48);      // Adjust this for your display orientation

    LCD_WriteCommand(0x3A);
    LCD_WriteData8(0x55);

    LCD_WriteCommand(0xB1);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x1B);

    LCD_WriteCommand(0xF2);    // 3Gamma Function Disable
    LCD_WriteData8(0x08);

    LCD_WriteCommand(0x26);    // Gamma curve selected
    LCD_WriteData8(0x01);

    LCD_WriteCommand(0xE0);    // Set Gamma
    LCD_WriteData8(0x1F);
    LCD_WriteData8(0x1A);
    LCD_WriteData8(0x18);
    LCD_WriteData8(0x0A);
    LCD_WriteData8(0x0F);
    LCD_WriteData8(0x06);
    LCD_WriteData8(0x45);
    LCD_WriteData8(0X87);
    LCD_WriteData8(0x32);
    LCD_WriteData8(0x0A);
    LCD_WriteData8(0x07);
    LCD_WriteData8(0x02);
    LCD_WriteData8(0x07);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x00);

    LCD_WriteCommand(0xE1);    // Set Gamma
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x25);
    LCD_WriteData8(0x27);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x10);
    LCD_WriteData8(0x09);
    LCD_WriteData8(0x3A);
    LCD_WriteData8(0x78);
    LCD_WriteData8(0x4D);
    LCD_WriteData8(0x05);
    LCD_WriteData8(0x18);
    LCD_WriteData8(0x0D);
    LCD_WriteData8(0x38);
    LCD_WriteData8(0x3A);
    LCD_WriteData8(0x1F);

    LCD_WriteCommand(0x2B);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x01);
    LCD_WriteData8(0x3f);

    LCD_WriteCommand(0x2A);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0x00);
    LCD_WriteData8(0xef);

    LCD_WriteCommand(0x11);    // Exit Sleep
    HAL_Delay(120);

    LCD_WriteCommand(0x29);    // Display on
    HAL_Delay(20);

    // Turn on backlight
    LCD_SetBacklight(1);
}

// Set window for drawing
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    LCD_WriteCommand(0x2A);
    LCD_WriteData8(x0 >> 8);
    LCD_WriteData8(x0 & 0xFF);
    LCD_WriteData8(x1 >> 8);
    LCD_WriteData8(x1 & 0xFF);

    LCD_WriteCommand(0x2B);
    LCD_WriteData8(y0 >> 8);
    LCD_WriteData8(y0 & 0xFF);
    LCD_WriteData8(y1 >> 8);
    LCD_WriteData8(y1 & 0xFF);

    LCD_WriteCommand(0x2C);    // Memory write
}


// Fill the entire screen with a color
void LCD_Fill(uint16_t color)
{
    LCD_SetWindow(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    for(uint32_t i = 0; i < LCD_WIDTH * LCD_HEIGHT; i++)
    {
        LCD_WriteData(color);
    }
}

// Draw a pixel at x,y position with specified color
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_SetWindow(x, y, x, y);
    LCD_WriteData(color);
}

// Draw a filled rectangle
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if((x >= LCD_WIDTH) || (y >= LCD_HEIGHT)) return;
    if((x + w - 1) >= LCD_WIDTH) w = LCD_WIDTH - x;
    if((y + h - 1) >= LCD_HEIGHT) h = LCD_HEIGHT - y;

    LCD_SetWindow(x, y, x + w - 1, y + h - 1);

    for(uint32_t i = 0; i < w * h; i++)
    {
        LCD_WriteData(color);
    }
}

// Draw a horizontal line
void LCD_DrawHLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    LCD_FillRect(x, y, length, 1, color);
}

// Draw a vertical line
void LCD_DrawVLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    LCD_FillRect(x, y, 1, length, color);
}


void LCD_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color)
{
    uint8_t temp, page, column;
    uint8_t relative_position;

    // Only handle printable ASCII characters
    if (ch < 32 || ch > 126) {
        return;
    }

    // Calculate font array index (space ' ' is first character)
    relative_position = ch - ' ';

    // Set drawing window for the character
    LCD_SetWindow(x, y, x + CHAR_WIDTH - 1, y + CHAR_HEIGHT - 1);

    // Draw character pixel by pixel
    for (page = 0; page < CHAR_HEIGHT; page++) {
        // Get one row of pixel data from font
        temp = ucAscii_1608[relative_position][page];

        for (column = 0; column < CHAR_WIDTH; column++) {
            // Check LSB first (your course code shifts right)
            if (temp & 0x01) {
                LCD_WriteData(color);      // Foreground pixel
            } else {
                LCD_WriteData(bg_color);   // Background pixel
            }

            temp >>= 1;  // Move to next pixel in the row
        }
    }
}

void LCD_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color)
{
    uint16_t current_x = x;
    uint16_t current_y = y;

    while (*str != '\0') {
        // Check if we need to wrap to next line
        if ((current_x + CHAR_WIDTH) > LCD_WIDTH) {
            current_x = x;                    // Return to start x
            current_y += CHAR_HEIGHT;         // Move to next line

            // Check if we've run out of vertical space
            if ((current_y + CHAR_HEIGHT) > LCD_HEIGHT) {
                break;  // No more space on screen
            }
        }

        // Draw the character
        LCD_DrawChar(current_x, current_y, *str, color, bg_color);

        // Move to next character position
        current_x += CHAR_WIDTH;
        str++;
    }
}

void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color) {
    int32_t radius_sq = radius * radius;

    for (int32_t y = -radius; y <= radius; y++) {
        int32_t y_sq = y * y;
        int32_t width = (int32_t)sqrt(radius_sq - y_sq);

        // Draw a horizontal line for each row
        for (int32_t x = -width; x <= width; x++) {
        	LCD_DrawPixel(x0 + x, y0 + y, color);
        }
    }
}

static void lcd_status(const char *title, const char *msg, uint16_t colour)
{
    LCD_Fill(LCD_COLOR_BLACK);
    LCD_DrawString(10, 10, (char*)title, LCD_COLOR_WHITE, LCD_COLOR_BLACK);

    /* centre the message */
    uint16_t msg_len = strlen(msg) * CHAR_WIDTH;
    uint16_t x = (LCD_WIDTH  - msg_len) / 2;
    LCD_DrawString(x, 50, (char*)msg, colour, LCD_COLOR_BLACK);
}





