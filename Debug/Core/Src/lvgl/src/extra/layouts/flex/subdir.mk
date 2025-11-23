################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/extra/layouts/flex/lv_flex.c 

OBJS += \
./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.o 

C_DEPS += \
./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/extra/layouts/flex/%.o Core/Src/lvgl/src/extra/layouts/flex/%.su Core/Src/lvgl/src/extra/layouts/flex/%.cyclo: ../Core/Src/lvgl/src/extra/layouts/flex/%.c Core/Src/lvgl/src/extra/layouts/flex/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/CMSIS/DSP/Include -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hcwon/STM32CubeIDE/workspace_1.19.0/AudioGraph_GUI_RTOS/Core/Src/lvgl" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-layouts-2f-flex

clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-layouts-2f-flex:
	-$(RM) ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.cyclo ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.d ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.o ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-layouts-2f-flex

