/*
 * lvgl_disp.h
 *
 *  Created on: Nov 14, 2025
 *      Author: hcwon
 */

#ifndef INC_LVGL_DISP_H_
#define INC_LVGL_DISP_H_

#include "lcd.h"
#include "lvgl.h"
#include "touch.h"

#define DISP_BUF_SIZE (LCD_WIDTH * 10)

void disp_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void lv_port_disp_init(void);


#endif /* INC_LVGL_DISP_H_ */
