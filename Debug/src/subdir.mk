################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CucuruchoLocal.cpp \
../src/Juego.cpp \
../src/cfg.cpp \
../src/registro.cpp 

OBJS += \
./src/CucuruchoLocal.o \
./src/Juego.o \
./src/cfg.o \
./src/registro.o 

CPP_DEPS += \
./src/CucuruchoLocal.d \
./src/Juego.d \
./src/cfg.d \
./src/registro.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -I/usr/include/libxml2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


