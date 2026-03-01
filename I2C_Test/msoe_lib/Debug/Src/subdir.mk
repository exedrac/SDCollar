################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/msoe_clk_setup.c \
../Src/msoe_stm_delay.c \
../Src/msoe_stm_lcd.c \
../Src/st7735.c \
../Src/stm32_adafruit_lcd.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/msoe_clk_setup.o \
./Src/msoe_stm_delay.o \
./Src/msoe_stm_lcd.o \
./Src/st7735.o \
./Src/stm32_adafruit_lcd.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/msoe_clk_setup.d \
./Src/msoe_stm_delay.d \
./Src/msoe_stm_lcd.d \
./Src/st7735.d \
./Src/stm32_adafruit_lcd.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U575xx -c -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/msoe_clk_setup.cyclo ./Src/msoe_clk_setup.d ./Src/msoe_clk_setup.o ./Src/msoe_clk_setup.su ./Src/msoe_stm_delay.cyclo ./Src/msoe_stm_delay.d ./Src/msoe_stm_delay.o ./Src/msoe_stm_delay.su ./Src/msoe_stm_lcd.cyclo ./Src/msoe_stm_lcd.d ./Src/msoe_stm_lcd.o ./Src/msoe_stm_lcd.su ./Src/st7735.cyclo ./Src/st7735.d ./Src/st7735.o ./Src/st7735.su ./Src/stm32_adafruit_lcd.cyclo ./Src/stm32_adafruit_lcd.d ./Src/stm32_adafruit_lcd.o ./Src/stm32_adafruit_lcd.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

