################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.c \
../Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.c \
../Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.c \
../Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.c \
../Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.c 

OBJS += \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.o \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.o \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.o \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.o \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.o 

C_DEPS += \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.d \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.d \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.d \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.d \
./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/extra/libs/fsdrv/%.o Core/Src/lvgl/src/extra/libs/fsdrv/%.su Core/Src/lvgl/src/extra/libs/fsdrv/%.cyclo: ../Core/Src/lvgl/src/extra/libs/fsdrv/%.c Core/Src/lvgl/src/extra/libs/fsdrv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Drivers/CMSIS/DSP/Include -I../FATFS/Target -I../FATFS/App -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/hcwon/STM32CubeIDE/workspace_1.19.0/AudioGraph_GUI_RTOS/Core/Src/lvgl" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv

clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv:
	-$(RM) ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.cyclo ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.d ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.o ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.su ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.cyclo ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.d ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.o ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_littlefs.su ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.cyclo ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.d ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.o ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_posix.su ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.cyclo ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.d ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.o ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.su ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.cyclo ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.d ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.o ./Core/Src/lvgl/src/extra/libs/fsdrv/lv_fs_win32.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-libs-2f-fsdrv

