/*
 * lvgl_disp.c
 *
 *  Created on: Nov 14, 2025
 *      Author: hcwon
 */
#include "lvgl_disp.h"
#include <string.h>

static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

static lv_color_t disp_buf1[DISP_BUF_SIZE];
static lv_disp_draw_buf_t draw_buf;

static Touch_Point_t curr_touch_point;

/*
 * Display driver flush callback
 * This function is called by LVGL when it's ready to flush pixels to the display
 */
void disp_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_buf) {
	//calculate area dim
	uint16_t width = area->x2 - area->x1 + 1;
	uint16_t height = area->y2 - area->y1 + 1;

	LCD_SetWindow(area->x1, area->y1, area->x2, area->y2);

	// Write pixel data to the LCD
	// Note: Since your LCD_DATA directly writes to memory-mapped location,
	// we can optimize the transfer by writing each pixel directly
	for (uint32_t i = 0; i < width * height; i++) {
		// LVGL color format is RGB565, same as your LCD
		LCD_WriteData(color_buf[i].full);
	}

	lv_disp_flush_ready(drv);
}

void touchpad_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    // Get the current touch point with calibration applied
    Touch_Get_Calibrated_Point(&curr_touch_point);

    // Set the touch state for LVGL
    if (curr_touch_point.pressed) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = curr_touch_point.x;
        data->point.y = curr_touch_point.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/*
 * Initialize the LVGL display driver
 */
void lvgl_display_init(void) {
	// Initialize the display buffer
	lv_disp_draw_buf_init(&draw_buf, disp_buf1, NULL, DISP_BUF_SIZE);

	// Initialize the display driver
	lv_disp_drv_init(&disp_drv);

	// Set display properties
	disp_drv.hor_res = LCD_WIDTH;
	disp_drv.ver_res = LCD_HEIGHT;
	disp_drv.flush_cb = disp_flush_cb;
	disp_drv.draw_buf = &draw_buf;

	// Register the display driver
	lv_disp_drv_register(&disp_drv);
}



void lvgl_touchpad_init(void) {
	// Check if touchscreen is calibrated, if not, perform calibration
	if (!Touch_Is_Calibrated()) {
		// Perform touch screen calibration
		if (Touch_Calibrate()) {
			// Save calibration to flash if successful
			Touch_Save_Calibration();
		}
	} else {
		// Load existing calibration data
		Touch_Load_Calibration();
	}

	// Initialize the input device driver
	lv_indev_drv_init(&indev_drv);

	 // Set input device properties
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = touchpad_read_cb;

	// Register the input device driver
	lv_indev_drv_register(&indev_drv);
}

void lvgl_port_init(void)
{
    // Initialize LVGL library
    lv_init();

    // Initialize LVGL display driver
    lvgl_display_init();

    // Initialize LVGL touchpad driver
    lvgl_touchpad_init();
}










