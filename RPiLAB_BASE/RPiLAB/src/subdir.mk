################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main.cpp \
../src/peripherals.cpp \
../src/utilities.cpp \
../src/z_starego.cpp 

OBJS += \
./src/main.o \
./src/peripherals.o \
./src/utilities.o \
./src/z_starego.o 

CPP_DEPS += \
./src/main.d \
./src/peripherals.d \
./src/utilities.d \
./src/z_starego.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-a7 -marm -O3 -I"C:\Users\wegne\Desktop\Studia\OSM-laby\gierka\RPiLAB_BASE\src" -I"C:\Users\wegne\Desktop\Studia\OSM-laby\gierka\RPiLAB_BASE\Library\include" -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


