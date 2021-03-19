################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Printer_Driver_Files/usbh_hid_test.c \
../Printer_Driver_Files/usbh_printer.c 

OBJS += \
./Printer_Driver_Files/usbh_hid_test.o \
./Printer_Driver_Files/usbh_printer.o 

C_DEPS += \
./Printer_Driver_Files/usbh_hid_test.d \
./Printer_Driver_Files/usbh_printer.d 


# Each subdirectory must supply rules for building sources it contributes
Printer_Driver_Files/usbh_hid_test.o: D:/Company_Projects_by_vivek/Axis_Company_Project/USB_HOST/Project_Code/TEST/Port_405/Printer_Driver_Files/usbh_hid_test.c Printer_Driver_Files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I"D:/Company_Projects_by_vivek/Axis_Company_Project/USB_HOST/Project_Code/TEST/Port_405/Printer_Driver_Files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Printer_Driver_Files/usbh_hid_test.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Printer_Driver_Files/usbh_printer.o: D:/Company_Projects_by_vivek/Axis_Company_Project/USB_HOST/Project_Code/TEST/Port_405/Printer_Driver_Files/usbh_printer.c Printer_Driver_Files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc -I"D:/Company_Projects_by_vivek/Axis_Company_Project/USB_HOST/Project_Code/TEST/Port_405/Printer_Driver_Files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Printer_Driver_Files/usbh_printer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

