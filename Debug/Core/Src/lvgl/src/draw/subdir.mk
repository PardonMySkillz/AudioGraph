################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/draw/lv_draw.c \
../Core/Src/lvgl/src/draw/lv_draw_arc.c \
../Core/Src/lvgl/src/draw/lv_draw_img.c \
../Core/Src/lvgl/src/draw/lv_draw_label.c \
../Core/Src/lvgl/src/draw/lv_draw_layer.c \
../Core/Src/lvgl/src/draw/lv_draw_line.c \
../Core/Src/lvgl/src/draw/lv_draw_mask.c \
../Core/Src/lvgl/src/draw/lv_draw_rect.c \
../Core/Src/lvgl/src/draw/lv_draw_transform.c \
../Core/Src/lvgl/src/draw/lv_draw_triangle.c \
../Core/Src/lvgl/src/draw/lv_img_buf.c \
../Core/Src/lvgl/src/draw/lv_img_cache.c \
../Core/Src/lvgl/src/draw/lv_img_decoder.c 

OBJS += \
./Core/Src/lvgl/src/draw/lv_draw.o \
./Core/Src/lvgl/src/draw/lv_draw_arc.o \
./Core/Src/lvgl/src/draw/lv_draw_img.o \
./Core/Src/lvgl/src/draw/lv_draw_label.o \
./Core/Src/lvgl/src/draw/lv_draw_layer.o \
./Core/Src/lvgl/src/draw/lv_draw_line.o \
./Core/Src/lvgl/src/draw/lv_draw_mask.o \
./Core/Src/lvgl/src/draw/lv_draw_rect.o \
./Core/Src/lvgl/src/draw/lv_draw_transform.o \
./Core/Src/lvgl/src/draw/lv_draw_triangle.o \
./Core/Src/lvgl/src/draw/lv_img_buf.o \
./Core/Src/lvgl/src/draw/lv_img_cache.o \
./Core/Src/lvgl/src/draw/lv_img_decoder.o 

C_DEPS += \
./Core/Src/lvgl/src/draw/lv_draw.d \
./Core/Src/lvgl/src/draw/lv_draw_arc.d \
./Core/Src/lvgl/src/draw/lv_draw_img.d \
./Core/Src/lvgl/src/draw/lv_draw_label.d \
./Core/Src/lvgl/src/draw/lv_draw_layer.d \
./Core/Src/lvgl/src/draw/lv_draw_line.d \
./Core/Src/lvgl/src/draw/lv_draw_mask.d \
./Core/Src/lvgl/src/draw/lv_draw_rect.d \
./Core/Src/lvgl/src/draw/lv_draw_transform.d \
./Core/Src/lvgl/src/draw/lv_draw_triangle.d \
./Core/Src/lvgl/src/draw/lv_img_buf.d \
./Core/Src/lvgl/src/draw/lv_img_cache.d \
./Core/Src/lvgl/src/draw/lv_img_decoder.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/draw/%.o Core/Src/lvgl/src/draw/%.su Core/Src/lvgl/src/draw/%.cyclo: ../Core/Src/lvgl/src/draw/%.c Core/Src/lvgl/src/draw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/CMSIS/DSP/Include -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hcwon/STM32CubeIDE/workspace_1.19.0/AudioGraph_GUI_RTOS/Core/Src/lvgl" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw

clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw:
	-$(RM) ./Core/Src/lvgl/src/draw/lv_draw.cyclo ./Core/Src/lvgl/src/draw/lv_draw.d ./Core/Src/lvgl/src/draw/lv_draw.o ./Core/Src/lvgl/src/draw/lv_draw.su ./Core/Src/lvgl/src/draw/lv_draw_arc.cyclo ./Core/Src/lvgl/src/draw/lv_draw_arc.d ./Core/Src/lvgl/src/draw/lv_draw_arc.o ./Core/Src/lvgl/src/draw/lv_draw_arc.su ./Core/Src/lvgl/src/draw/lv_draw_img.cyclo ./Core/Src/lvgl/src/draw/lv_draw_img.d ./Core/Src/lvgl/src/draw/lv_draw_img.o ./Core/Src/lvgl/src/draw/lv_draw_img.su ./Core/Src/lvgl/src/draw/lv_draw_label.cyclo ./Core/Src/lvgl/src/draw/lv_draw_label.d ./Core/Src/lvgl/src/draw/lv_draw_label.o ./Core/Src/lvgl/src/draw/lv_draw_label.su ./Core/Src/lvgl/src/draw/lv_draw_layer.cyclo ./Core/Src/lvgl/src/draw/lv_draw_layer.d ./Core/Src/lvgl/src/draw/lv_draw_layer.o ./Core/Src/lvgl/src/draw/lv_draw_layer.su ./Core/Src/lvgl/src/draw/lv_draw_line.cyclo ./Core/Src/lvgl/src/draw/lv_draw_line.d ./Core/Src/lvgl/src/draw/lv_draw_line.o ./Core/Src/lvgl/src/draw/lv_draw_line.su ./Core/Src/lvgl/src/draw/lv_draw_mask.cyclo ./Core/Src/lvgl/src/draw/lv_draw_mask.d ./Core/Src/lvgl/src/draw/lv_draw_mask.o ./Core/Src/lvgl/src/draw/lv_draw_mask.su ./Core/Src/lvgl/src/draw/lv_draw_rect.cyclo ./Core/Src/lvgl/src/draw/lv_draw_rect.d ./Core/Src/lvgl/src/draw/lv_draw_rect.o ./Core/Src/lvgl/src/draw/lv_draw_rect.su ./Core/Src/lvgl/src/draw/lv_draw_transform.cyclo ./Core/Src/lvgl/src/draw/lv_draw_transform.d ./Core/Src/lvgl/src/draw/lv_draw_transform.o ./Core/Src/lvgl/src/draw/lv_draw_transform.su ./Core/Src/lvgl/src/draw/lv_draw_triangle.cyclo ./Core/Src/lvgl/src/draw/lv_draw_triangle.d ./Core/Src/lvgl/src/draw/lv_draw_triangle.o ./Core/Src/lvgl/src/draw/lv_draw_triangle.su ./Core/Src/lvgl/src/draw/lv_img_buf.cyclo ./Core/Src/lvgl/src/draw/lv_img_buf.d ./Core/Src/lvgl/src/draw/lv_img_buf.o ./Core/Src/lvgl/src/draw/lv_img_buf.su ./Core/Src/lvgl/src/draw/lv_img_cache.cyclo ./Core/Src/lvgl/src/draw/lv_img_cache.d ./Core/Src/lvgl/src/draw/lv_img_cache.o ./Core/Src/lvgl/src/draw/lv_img_cache.su ./Core/Src/lvgl/src/draw/lv_img_decoder.cyclo ./Core/Src/lvgl/src/draw/lv_img_decoder.d ./Core/Src/lvgl/src/draw/lv_img_decoder.o ./Core/Src/lvgl/src/draw/lv_img_decoder.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw

