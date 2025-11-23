/*
 * touch.h
 *
 *  Created on: Nov 2, 2025
 *      Author: hcwon
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define TOUCH_CS_PORT GPIOB
#define TOUCH_CS_PIN GPIO_PIN_12
#define TOUCH_PEN_PORT GPIOC
#define TOUCH_PEN_PIN GPIO_PIN_5

#define CMD_READ_X  0xD0
#define CMD_READ_Y  0x90

#define LCD_WIDTH   240
#define LCD_HEIGHT  320

#define CAL_POINT1_X 20
#define CAL_POINT1_Y 20
#define CAL_POINT2_X (LCD_WIDTH - 20)
#define CAL_POINT2_Y 20
#define CAL_POINT3_X (LCD_WIDTH / 2)
#define CAL_POINT3_Y (LCD_HEIGHT - 20)

#define CALIBRATION_SECTOR      FLASH_SECTOR_11
#define CALIBRATION_ADDRESS     0x080E0000

typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t pressed;
} Touch_Point_t;

typedef struct {
    int64_t A;
    int64_t B;
    int64_t C;
    int64_t D;
    int64_t E;
    int64_t F;
    int64_t div;
} Touch_Calibration_t;


extern SPI_HandleTypeDef hspi2;
extern volatile uint8_t touch_pressed_flag;


// Touch Functions
void Touch_Init(void);
uint8_t Touch_Is_Pressed(void);
uint16_t Touch_Read_ADC(uint8_t cmd);
void Touch_Get_Point(Touch_Point_t* point);

// Calibration functions
uint8_t Touch_Is_Calibrated(void);
uint8_t Touch_Calibrate(void);
void Touch_Save_Calibration(void);
uint8_t Touch_Load_Calibration(void);
void Touch_Get_Calibrated_Point(Touch_Point_t* point);



#define SETTLE_TIME_MS    12
#define SAMPLE_INTERVAL_MS 8


#endif /* INC_TOUCH_H_ */
