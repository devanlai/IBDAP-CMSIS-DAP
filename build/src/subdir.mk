################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/DAP.c \
../src/IBDAP.c \
../src/JTAG_DP.c \
../src/SW_DP.c \
../src/cr_startup_lpc11uxx.c \
../src/crp.c \
../src/system_LPC11Uxx.c \
../src/uart.c \
../src/usb_desc.c \
../src/usb_driver.c 

OBJS += \
./src/DAP.o \
./src/IBDAP.o \
./src/JTAG_DP.o \
./src/SW_DP.o \
./src/cr_startup_lpc11uxx.o \
./src/crp.o \
./src/system_LPC11Uxx.o \
./src/uart.o \
./src/usb_desc.o \
./src/usb_driver.o 

C_DEPS += \
./src/DAP.d \
./src/IBDAP.d \
./src/JTAG_DP.d \
./src/SW_DP.d \
./src/cr_startup_lpc11uxx.d \
./src/crp.d \
./src/system_LPC11Uxx.d \
./src/uart.d \
./src/usb_desc.d \
./src/usb_driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DNDEBUG  -DCORE_M0 -D__USE_CMSIS -D__LPC11UXX__ -I"/home/yliu/projects/ib51822/IBDAP-FW/Firmware/IBDAP/inc" -I"/home/yliu/projects/ib51822/IBDAP-FW/Firmware/IBDAP/inc/usbd" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/system_LPC11Uxx.o: ../src/system_LPC11Uxx.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DNDEBUG -DCORE_M0 -D__USE_CMSIS -D__LPC11UXX__ -I"/home/yliu/projects/ib51822/IBDAP-FW/Firmware/IBDAP/inc" -I"/home/yliu/projects/ib51822/IBDAP-FW/Firmware/IBDAP/inc/usbd" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/system_LPC11Uxx.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


