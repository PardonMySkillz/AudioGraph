################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.c 

OBJS += \
./Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.o 

C_DEPS += \
./Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/extra/libs/ffmpeg/%.o Core/Src/lvgl/src/extra/libs/ffmpeg/%.su Core/Src/lvgl/src/extra/libs/ffmpeg/%.cyclo: ../Core/Src/lvgl/src/extra/libs/ffmpeg/%.c Core/Src/lvgl/src/extra/libs/ffmpeg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/CMSIS/DSP/Include -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hcwon/STM32CubeIDE/workspace_1.19.0/AudioGraph_GUI_RTOS/Core/Src/lvgl" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-libs-2f-ffmpeg

clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-libs-2f-ffmpeg:
	-$(RM) ./Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.cyclo ./Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.d ./Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.o ./Core/Src/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-libs-2f-ffmpeg

