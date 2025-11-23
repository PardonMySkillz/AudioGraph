/*
 * touch.c
 *
 *  Created on: Nov 2, 2025
 *      Author: hcwon
 */
#include "touch.h"
#include "main.h"
#include "lcd.h"

static Touch_Calibration_t calibration;
static uint8_t is_calibrated = 0;
volatile uint8_t touch_pressed_flag = 0;

static uint16_t Touch_SPI_Exchange(uint8_t cmd) {
    uint8_t tx_data[3] = {cmd, 0x00, 0x00};  // Command + 2 dummy bytes
    uint8_t rx_data[3] = {0};

    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_RESET);  // CS low
    HAL_SPI_TransmitReceive(&hspi2, tx_data, rx_data, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);    // CS high

    // Combine high/low bytes (12-bit: discard first byte, bits 11-0)
    return ((rx_data[1] << 8) | rx_data[2]) >> 3;  // Shift to 12-bit
}

// Initialize touch (GPIO for CS and PEN)
void Touch_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // CS pin: Output PP
    GPIO_InitStruct.Pin = TOUCH_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(TOUCH_CS_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TOUCH_CS_PORT, TOUCH_CS_PIN, GPIO_PIN_SET);  // CS high (idle)

    // PEN pin: Input with pull-up (interrupt low when touched)
    GPIO_InitStruct.Pin = TOUCH_PEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TOUCH_PEN_PORT, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);   // Adjust priority as needed
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Check if touched (T_PEN low)
uint8_t Touch_Is_Pressed(void) {
	return (HAL_GPIO_ReadPin(TOUCH_PEN_PORT, TOUCH_PEN_PIN) == GPIO_PIN_RESET);
}

// Read raw ADC value for X or Y
uint16_t Touch_Read_ADC(uint8_t cmd) {
    return Touch_SPI_Exchange(cmd);
}

// Get touch point (average 5 readings for noise reduction)
void Touch_Get_Point(Touch_Point_t* point) {
    point->pressed = Touch_Is_Pressed();
    if (!point->pressed) {
        point->x = 0;
        point->y = 0;
        return;
    }

    uint32_t sum_x = 0, sum_y = 0;
    uint8_t samples = 5;  // Average for stability

    for (uint8_t i = 0; i < samples; i++) {
        sum_x += Touch_Read_ADC(CMD_READ_X);
        sum_y += Touch_Read_ADC(CMD_READ_Y);
    }

    uint16_t raw_x = sum_x / samples;  // Raw 0-4095
    uint16_t raw_y = sum_y / samples;  // Raw 0-4095

    // Swap axes for portrait orientation (adjust based on your panel)
    point->x = raw_y;  // Map raw Y to screen X (horizontal)
    point->y = 4095 - raw_x;  // Map inverted raw X to screen Y (vertical) - test without '4095 -' if direction is wrong
}

void Touch_Get_Calibrated_Point(Touch_Point_t* point) {
    Touch_Get_Point(point);

    if (!point->pressed || !is_calibrated) {
        return;  // Return raw point if not pressed or not calibrated
    }

    // Apply calibration matrix transformation
    int64_t x = point->x;
    int64_t y = point->y;

    // Calculate calibrated coordinates
    int64_t cal_x = (calibration.A * x + calibration.B * y + calibration.C) / calibration.div;
    int64_t cal_y = (calibration.D * x + calibration.E * y + calibration.F) / calibration.div;

    // Boundary check
    if (cal_x < 0) cal_x = 0;
    if (cal_x >= LCD_WIDTH) cal_x = LCD_WIDTH - 1;
    if (cal_y < 0) cal_y = 0;
    if (cal_y >= LCD_HEIGHT) cal_y = LCD_HEIGHT - 1;

    // Update with calibrated values
    point->x = (uint16_t)cal_x;
    point->y = (uint16_t)cal_y;
}

uint8_t Touch_Is_Calibrated(void) {
    return is_calibrated;
}

// 3-point calibration
uint8_t Touch_Calibrate(void) {
    Touch_Point_t p1, p2, p3;
    int64_t dx1, dx2, dx3, dy1, dy2, dy3;
    int64_t tx1, tx2, tx3, ty1, ty2, ty3;

    // Clear screen and draw first calibration point
    LCD_Fill(LCD_COLOR_BLACK);
    LCD_DrawString(LCD_WIDTH/2 - 80, LCD_HEIGHT/2 - 10, "Touchscreen Calibration", LCD_COLOR_WHITE, LCD_COLOR_BLACK);
    LCD_DrawString(LCD_WIDTH/2 - 90, LCD_HEIGHT/2 + 10, "Touch the points as they appear", LCD_COLOR_WHITE, LCD_COLOR_BLACK);
    HAL_Delay(500);

    // Point 1 (top-left)
    LCD_Fill(LCD_COLOR_BLACK);
    LCD_DrawCircle(CAL_POINT1_X, CAL_POINT1_Y, 5, LCD_COLOR_RED);
    LCD_DrawCircle(CAL_POINT1_X, CAL_POINT1_Y, 2, LCD_COLOR_WHITE);

    // Wait for touch and release
    while (!Touch_Is_Pressed()) HAL_Delay(10);
    Touch_Get_Point(&p1);
    while (Touch_Is_Pressed()) HAL_Delay(10);
    HAL_Delay(500);

    // Point 2 (top-right)
    LCD_Fill(LCD_COLOR_BLACK);
    LCD_DrawCircle(CAL_POINT2_X, CAL_POINT2_Y, 5, LCD_COLOR_RED);
    LCD_DrawCircle(CAL_POINT2_X, CAL_POINT2_Y, 2, LCD_COLOR_WHITE);

    while (!Touch_Is_Pressed()) HAL_Delay(10);
    Touch_Get_Point(&p2);
    while (Touch_Is_Pressed()) HAL_Delay(10);
    HAL_Delay(500);

    // Point 3 (bottom-middle)
    LCD_Fill(LCD_COLOR_BLACK);
    LCD_DrawCircle(CAL_POINT3_X, CAL_POINT3_Y, 5, LCD_COLOR_RED);
    LCD_DrawCircle(CAL_POINT3_X, CAL_POINT3_Y, 2, LCD_COLOR_WHITE);

    while (!Touch_Is_Pressed()) HAL_Delay(10);
    Touch_Get_Point(&p3);
    while (Touch_Is_Pressed()) HAL_Delay(10);

    // Calibration matrix calculation
    tx1 = CAL_POINT1_X;
    ty1 = CAL_POINT1_Y;
    tx2 = CAL_POINT2_X;
    ty2 = CAL_POINT2_Y;
    tx3 = CAL_POINT3_X;
    ty3 = CAL_POINT3_Y;

    dx1 = p1.x;
    dy1 = p1.y;
    dx2 = p2.x;
    dy2 = p2.y;
    dx3 = p3.x;
    dy3 = p3.y;

    // Calculate matrix coefficients using int64_t to avoid overflow
    calibration.div = (dx1 - dx3) * (dy2 - dy3) - (dx2 - dx3) * (dy1 - dy3);

    // Check if valid (non-zero determinant)
    if (calibration.div == 0) {
        LCD_Fill(LCD_COLOR_BLACK);
        LCD_DrawString(LCD_WIDTH/2 - 60, LCD_HEIGHT/2, "Calibration Failed!", LCD_COLOR_RED, LCD_COLOR_BLACK);
        LCD_DrawString(LCD_WIDTH/2 - 70, LCD_HEIGHT/2 + 20, "Please try again.", LCD_COLOR_WHITE, LCD_COLOR_BLACK);
        HAL_Delay(2000);
        return 0;
    }

    calibration.A = ((tx1 - tx3) * (dy2 - dy3) - (tx2 - tx3) * (dy1 - dy3));
    calibration.B = ((dx1 - dx3) * (tx2 - tx3) - (dx2 - dx3) * (tx1 - tx3));
    calibration.C = (dx1 * (tx3 * dy2 - tx2 * dy3) + dx2 * (tx1 * dy3 - tx3 * dy1) + dx3 * (tx2 * dy1 - tx1 * dy2));

    calibration.D = ((ty1 - ty3) * (dy2 - dy3) - (ty2 - ty3) * (dy1 - dy3));
    calibration.E = ((dx1 - dx3) * (ty2 - ty3) - (dx2 - dx3) * (ty1 - ty3));
    calibration.F = (dx1 * (ty3 * dy2 - ty2 * dy3) + dx2 * (ty1 * dy3 - ty3 * dy1) + dx3 * (ty2 * dy1 - ty1 * dy2));

    is_calibrated = 1;

    // Save calibration data to flash
    Touch_Save_Calibration();

    // Display success message
    LCD_Fill(LCD_COLOR_BLACK);
    LCD_DrawString(LCD_WIDTH/2 - 60, LCD_HEIGHT/2, "Calibration Successful!", LCD_COLOR_GREEN, LCD_COLOR_BLACK);
    HAL_Delay(500);

    return 1;
}

// Save calibration data to flash
void Touch_Save_Calibration(void) {
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;
    uint32_t data[sizeof(Touch_Calibration_t)/4 + 1]; // +1 for validation

    // Prepare data array
    memcpy(data, &calibration, sizeof(Touch_Calibration_t));
    data[sizeof(Touch_Calibration_t)/4] = 0xA5A5A5A5; // Validation pattern

    // Unlock flash
    HAL_FLASH_Unlock();

    // Erase sector
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = CALIBRATION_SECTOR;
    EraseInitStruct.NbSectors = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) == HAL_OK) {
        // Write data
        for (uint32_t i = 0; i < (sizeof(Touch_Calibration_t)/4 + 1); i++) {
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, CALIBRATION_ADDRESS + (i*4), data[i]);
        }
    }

    // Lock flash
    HAL_FLASH_Lock();
}

// Load calibration data from flash
uint8_t Touch_Load_Calibration(void) {
    uint32_t* flash_data = (uint32_t*)CALIBRATION_ADDRESS;
    uint32_t validation = *(uint32_t*)(CALIBRATION_ADDRESS + sizeof(Touch_Calibration_t));

    // Check validation pattern
    if (validation == 0xA5A5A5A5) {
        memcpy(&calibration, flash_data, sizeof(Touch_Calibration_t));
        is_calibrated = 1;
        return 1;
    }

    // No valid calibration data
    is_calibrated = 0;
    return 0;
}

volatile uint32_t last_touch_time = 0;
#define DEBOUNCE_MS 30
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == TOUCH_PEN_PIN)
    {
        uint32_t now = HAL_GetTick();
        if (now - last_touch_time > DEBOUNCE_MS)
        {
            last_touch_time = now;
            touch_pressed_flag = 1;
        }
    }
}




