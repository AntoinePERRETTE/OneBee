################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DS18B20.c \
../Src/FLASH.c \
../Src/HX711.c \
../Src/LORAWAN.c \
../Src/SleepMode.c \
../Src/gpio.c \
../Src/interrupt.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer.c 

OBJS += \
./Src/DS18B20.o \
./Src/FLASH.o \
./Src/HX711.o \
./Src/LORAWAN.o \
./Src/SleepMode.o \
./Src/gpio.o \
./Src/interrupt.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer.o 

C_DEPS += \
./Src/DS18B20.d \
./Src/FLASH.d \
./Src/HX711.d \
./Src/LORAWAN.d \
./Src/SleepMode.d \
./Src/gpio.d \
./Src/interrupt.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G0 -DSTM32G031K8Tx -c -I../Inc -I"/home/perrettea/Documents/L3 chambery/spacebee/Space-Bee-main/Equipe1_RucheConnecte_V2/Drivers/CMSIS/Include" -I"/home/perrettea/Documents/L3 chambery/spacebee/Space-Bee-main/Equipe1_RucheConnecte_V2/Drivers/CMSIS/Device/ST/STM32G0xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/DS18B20.cyclo ./Src/DS18B20.d ./Src/DS18B20.o ./Src/DS18B20.su ./Src/FLASH.cyclo ./Src/FLASH.d ./Src/FLASH.o ./Src/FLASH.su ./Src/HX711.cyclo ./Src/HX711.d ./Src/HX711.o ./Src/HX711.su ./Src/LORAWAN.cyclo ./Src/LORAWAN.d ./Src/LORAWAN.o ./Src/LORAWAN.su ./Src/SleepMode.cyclo ./Src/SleepMode.d ./Src/SleepMode.o ./Src/SleepMode.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/interrupt.cyclo ./Src/interrupt.d ./Src/interrupt.o ./Src/interrupt.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su

.PHONY: clean-Src

