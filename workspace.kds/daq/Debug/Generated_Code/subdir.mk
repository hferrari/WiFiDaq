################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/Cpu.c \
../Generated_Code/I2C0.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/Pins1.c \
../Generated_Code/Pot.c \
../Generated_Code/RTC.c \
../Generated_Code/UART_ESP8266.c \
../Generated_Code/UART_SDA.c 

OBJS += \
./Generated_Code/Cpu.o \
./Generated_Code/I2C0.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/Pins1.o \
./Generated_Code/Pot.o \
./Generated_Code/RTC.o \
./Generated_Code/UART_ESP8266.o \
./Generated_Code/UART_SDA.o 

C_DEPS += \
./Generated_Code/Cpu.d \
./Generated_Code/I2C0.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/Pins1.d \
./Generated_Code/Pot.d \
./Generated_Code/RTC.d \
./Generated_Code/UART_ESP8266.d \
./Generated_Code/UART_SDA.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/home/henrique/workspace.kds/daq/Static_Code/System" -I"/home/henrique/workspace.kds/daq/Static_Code/PDD" -I"/home/henrique/workspace.kds/daq/Static_Code/IO_Map" -I"/opt/Freescale/KDS_3.0.0/eclipse/ProcessorExpert/lib/Kinetis/pdd/inc" -I"/home/henrique/workspace.kds/daq/Sources" -I"/home/henrique/workspace.kds/daq/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


