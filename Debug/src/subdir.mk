################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CucuruchoLocal.cpp \
../src/EntidadDibujable.cpp \
../src/Juego.cpp \
../src/Personaje.cpp \
../src/cfg.cpp \
../src/iocontra.cpp \
../src/juego.cpp \
../src/plataformas.cpp \
../src/registro.cpp \
../src/temporizador.cpp 

OBJS += \
./src/CucuruchoLocal.o \
./src/EntidadDibujable.o \
./src/Juego.o \
./src/Personaje.o \
./src/cfg.o \
./src/iocontra.o \
./src/juego.o \
./src/plataformas.o \
./src/registro.o \
./src/temporizador.o 

CPP_DEPS += \
./src/CucuruchoLocal.d \
./src/EntidadDibujable.d \
./src/Juego.d \
./src/Personaje.d \
./src/cfg.d \
./src/iocontra.d \
./src/juego.d \
./src/plataformas.d \
./src/registro.d \
./src/temporizador.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -I/usr/include/libxml2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


